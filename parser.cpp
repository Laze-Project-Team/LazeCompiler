#include "parser.hpp"
#include <nlohmann/json.hpp>
#include <unordered_map>
#include <deque>
#include <set>
#include <vector>
#include <list>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <string>
#include <regex> 
#include <sstream>
#include <cstring>

static std::string tokenNames[] = {"char","string","hex","int","real",",",":",";","(",")","[","]","{","}",".","->","<-","=>","+","-","*","/","==","!=","<=","<",">=",">","&&","||","=","if","then","else","from","to","break","inttype","realtype","continue","return","type","void","nul","true","false","boolean","chartype","%","&","shorttype","function","loop","jsload","sizeof","class","private","public","protected","repeat","jsexport","id","uminus","lower_than_else", "eof", ""};
static std::string nonTerminal[] = {"exp", "var", "varExp", "dec", "stm", "ty", "field", "explist", "stmlist", "declist", "memlist", "fieldlist", "oper", "funcAndVar", "funcAndVarList", "mems"};

using grammarListTy = std::unordered_map<std::string, std::deque<std::string>>;
using grammarTy = std::deque<std::string>;
using itemSetTy = std::vector<std::pair<std::string, std::deque<std::string>>>;
using firstSetTy = std::map<std::string, std::set<std::string>>;
using tokensTy = std::set<std::string>;
using tableTy = std::map<std::string, std::vector<std::string>>;

static unsigned int itemSetNumber = 0; 
static std::vector<std::unordered_map<std::string, int>> transitionTable;

// takes a rule and gives the itemSet needed to define the nonterminals
static itemSetTy addRules(std::deque<std::string> rule, const grammarListTy &grammarList, itemSetTy pastItems){
    itemSetTy items;
    auto pointerIt = std::find(rule.begin(), rule.end(), ">>>");
    if(pointerIt != rule.end() && pointerIt + 1 != rule.end()){
        pointerIt++;
    }
    for(const auto &i:pastItems){
        if(*pointerIt == i.first || *pointerIt == i.first.substr(0, i.first.find('.'))){
            return items;
        }
    }
    if(std::find(std::begin(nonTerminal), std::end(nonTerminal), *pointerIt) != std::end(nonTerminal)){
        for(const auto &i: grammarList){
            std::string tokName = i.first.substr(0, i.first.find('.'));
            if(*pointerIt == tokName){
                grammarTy newGrammar = i.second;
                newGrammar.push_front(">>>");
                items.push_back(std::make_pair(i.first, newGrammar));
                pastItems.push_back(std::make_pair(i.first, newGrammar));
                itemSetTy newRule = addRules(newGrammar, grammarList, pastItems);
                if(newRule.size() > 0){
                    items.insert(items.end(), newRule.begin(), newRule.end());
                    pastItems.insert(pastItems.end(), newRule.begin(), newRule.end());
                }
            }
        }
    }
    // std::cout << items.size() << std::endl;
    return items;
}

static std::vector<itemSetTy> toItem0(const grammarListTy &grammarList){
    std::vector<itemSetTy> itemSets;
    grammarTy rule = grammarList.at("start");
    rule.push_front(">>>");
    itemSets.push_back(itemSetTy());
    itemSets.back().push_back(std::make_pair("start", rule));
    itemSets.back().push_back(std::make_pair("additionalRules", grammarTy({"-------------------------------"})));
    itemSetTy items = addRules(rule, grammarList, itemSets.back());
    itemSets.back().insert(itemSets.back().end(), items.begin(), items.end());
    return itemSets;
}

// takes token input and returns new itemSet
static itemSetTy takeInput(itemSetTy itemSetInput, std::string token){
    itemSetTy result;
    for(auto item: itemSetInput){
        grammarTy rule = item.second;
        auto pointerIt = std::find(rule.begin(), rule.end(), ">>>");
        if(pointerIt != rule.end() && pointerIt != rule.end() - 1 && *(pointerIt + 1) == token){
            std::iter_swap(pointerIt, pointerIt + 1);
            result.push_back(std::make_pair(item.first, rule));
        }
    }
    return result;
}

//combine nonTerminal rules
static itemSetTy toNonTerminalDef(itemSetTy itemSetParam, const grammarListTy &grammarList){
    itemSetTy result;
    for(const auto &item: itemSetParam){
        itemSetTy itemSet = addRules(item.second, grammarList, result);
        result.insert(result.end(), itemSet.begin(), itemSet.end());
    }
    return result;
}

// takes itemSet and returns all itemSets that was able to take an input
static std::vector<itemSetTy> toItemSets(itemSetTy itemSetParam, const grammarListTy &grammarList){
    std::vector<itemSetTy> result;
    unsigned tokenNum = 0;
    for(const auto &tokenName: tokenNames){
        itemSetTy newItemSet = takeInput(itemSetParam, tokenName);
        if(newItemSet.size() > 0){
            itemSetTy nonTerminalDefs = toNonTerminalDef(newItemSet, grammarList);
            newItemSet.push_back(std::make_pair("additionalRules", std::deque<std::string>({"-------------------"})));
            newItemSet.insert(newItemSet.end(), nonTerminalDefs.begin(), nonTerminalDefs.end());
            result.push_back(newItemSet);
        }
    }
    for(const auto &nonTerminalName: nonTerminal){
        itemSetTy newItemSet = takeInput(itemSetParam, nonTerminalName);
        if(newItemSet.size() > 0){
            tokenNum++;
            itemSetTy nonTerminalDefs = toNonTerminalDef(newItemSet, grammarList);
            newItemSet.push_back(std::make_pair("additionalRules", std::deque<std::string>({"-------------------"})));
            newItemSet.insert(newItemSet.end(), nonTerminalDefs.begin(), nonTerminalDefs.end());
            result.push_back(newItemSet);
        }
    }
    return result;
}

// takes multiple itemSets and returns all itemSets that took one input
/*
 * @param itemSets the itemSets you want to process (itemSets already exist in nowSets)
 * @param nowSets the itemSets that exists now
 * @param grammarList the grammar rules
 */
static std::vector<itemSetTy> toItemSets_multi(std::vector<itemSetTy> itemSets, std::vector<itemSetTy> nowSets, const grammarListTy &grammarList){
    std::vector<itemSetTy> result;
    unsigned setNum = 0;
    // the total number of nowSets excluding itemSets
    unsigned ii = nowSets.size() - itemSets.size();
    // std::cout << itemSets.size() << "toItemSets_multi start sets" << std::endl;
    // insert empty lines the size of itemSets
    for(int i = 0; i < itemSets.size(); i++){
        transitionTable.push_back(std::unordered_map<std::string, int>());
    }
    // when pushing the same token input, the goal itemSet must be the same.
    for(const auto &itemSet: itemSets){
        std::vector<itemSetTy> output = toItemSets(itemSet, grammarList);
        for(auto &outputSet: output){
            std::deque<int> acceptableItemSets(nowSets.size());
            std::iota(acceptableItemSets.begin(), acceptableItemSets.end(), 0);
            int thisIncludes = -1;
            for(auto it = acceptableItemSets.begin(); it != acceptableItemSets.end(); it++){
                if(nowSets.at(*it) == outputSet){
                    thisIncludes = *it;
                    break;
                }
            }

            auto pointerIt = std::find(outputSet.front().second.begin(), outputSet.front().second.end(), ">>>");
            if(thisIncludes >= 0){
                transitionTable.at(ii)[*(pointerIt - 1)] = thisIncludes;
            }
            else if(thisIncludes == -1){
                transitionTable.at(ii)[*(pointerIt - 1)] = nowSets.size();
                setNum++;
                result.push_back(outputSet);
                nowSets.push_back(outputSet);
            }
        }
        ii++;
    }
    return result;
}

static std::vector<itemSetTy> createAllItemSets(std::vector<itemSetTy> itemSet0, const grammarListTy &grammarList){
    std::vector<itemSetTy> result;
    result.insert(result.end(), itemSet0.begin(), itemSet0.end());
    std::vector<itemSetTy> itemSets = toItemSets_multi(itemSet0, result, grammarList);
    // std::cout << itemSets.size() << "from itemSet0"<< std::endl;
    while(itemSets.size() > 0){
        result.insert(result.end(), itemSets.begin(), itemSets.end());
        itemSets = toItemSets_multi(itemSets, result, grammarList);
        // std::cout << itemSets.size() << std::endl;
    }
    return result;
}

static itemSetTy toExtendedGrammar(std::vector<itemSetTy> itemSets){
    itemSetTy result;
    unsigned itemSetNum = 0;
    for(const auto &itemSet:itemSets){
        for(const auto &item:itemSet){
            if(item.second.front() == ">>>"){
                grammarTy newGrammar;
                std::string tokName = item.first.substr(0, item.first.find('.'));
                std::string ruleName = std::to_string(itemSetNum) + " " + tokName + " " + std::to_string(transitionTable.at(itemSetNum)[tokName]);
                if(tokName == "start"){
                    ruleName = "0 start eof";
                }
                unsigned ruleItemSetNum = itemSetNum; 
                for(const auto &tok: item.second){
                    if(tok == ">>>"){
                        continue;
                    }
                    std::string rn = std::to_string(ruleItemSetNum) + " " + tok + " " + std::to_string(transitionTable.at(ruleItemSetNum)[tok]);
                    ruleItemSetNum = transitionTable.at(ruleItemSetNum)[tok];
                    newGrammar.push_back(rn);
                }
                result.push_back(std::make_pair(ruleName, newGrammar));
            }
        }
        itemSetNum++;
    }
    return result;
}

static tokensTy firstToken(grammarTy grammar, firstSetTy nowFIRST, const itemSetTy &extended){
    tokensTy result;
    std::stringstream tokStream(grammar.front());
    std::string tokName, startSetNum, endSetNum;
    tokStream >> startSetNum >> tokName >> endSetNum;
    if(endSetNum == ""){
        result.insert("");
        return result;
    }
    if(std::find(std::begin(tokenNames), std::end(tokenNames), tokName) != std::end(tokenNames)){
        result.insert(tokName);
        return result;
    }
    else if(std::find(std::begin(nonTerminal), std::end(nonTerminal), tokName) != std::end(nonTerminal)){
        if(nowFIRST.find(grammar.front()) != nowFIRST.end()){
            result = nowFIRST.at(grammar.front());
            return result;
        }
        else{
            nowFIRST[grammar.front()] = tokensTy({});
            for(const auto &item: extended){
                if(item.first == grammar.front()){
                    // std::cout << "found in extended" << std::endl;
                    tokensTy newFIRSTset = firstToken(item.second, nowFIRST, extended);
                    result.merge(newFIRSTset);
                }
            }
            return result;
        }
    }
    else{
        std::cerr << "what is this token" << grammar.front() << std::endl;
        exit(0);
    }
}

static firstSetTy generateFirstSet(const itemSetTy &extended){
    firstSetTy result;
    for(const auto &rule: extended){
        std::stringstream tokStream(rule.second.front());
        std::string tokName, simpleTokName;
        tokStream >> simpleTokName >> simpleTokName;
        tokensTy toMerge = firstToken(rule.second, result, extended);
        result[rule.first].merge(toMerge);
    }
    return result;
}

static firstSetTy generateFollowSet(const itemSetTy &extended, firstSetTy FIRSTset){
    firstSetTy FOLLOWset;
    FOLLOWset["0 start eof"] = tokensTy({"eof"});
    for(const auto &rule: extended){
        // iterate through every token in the extended rule
        for(int i = 0; i < rule.second.size(); i++){
            // get the next token
            std::string nextExtendedToken = rule.second.at(i);
            std::stringstream tokStream(nextExtendedToken);
            std::string nextToken;
            tokStream >> nextToken >> nextToken;
            std::string nextSimpleToken = nextToken.substr(0, nextToken.find('.'));

            if(std::find(std::begin(tokenNames), std::end(tokenNames), nextSimpleToken) != std::end(tokenNames)){
                continue;
            }
            else if(std::find(std::begin(nonTerminal), std::end(nonTerminal), nextSimpleToken) != std::end(nonTerminal)){
                if(i == rule.second.size() - 1){
                    if(rule.second.at(i) != rule.first){
                        tokensTy toMerge = FOLLOWset[rule.first];
                        FOLLOWset[nextExtendedToken].merge(toMerge);
                    }
                }
                else{
                    std::string nextNextExtendedToken = rule.second.at(i + 1);
                    std::stringstream tokStream(nextNextExtendedToken);
                    std::string nextNextToken, startItemSet, endItemSet;
                    tokStream >> startItemSet >> nextNextToken >> endItemSet;
                    std::string nextNextSimpleToken = nextNextToken.substr(0, nextNextToken.find('.'));
                    if(nextNextToken == "."){
                        nextNextSimpleToken = ".";
                    }
                    if(std::find(std::begin(tokenNames), std::end(tokenNames), nextNextSimpleToken) != std::end(tokenNames)){
                        FOLLOWset[nextExtendedToken].insert(nextNextSimpleToken);
                    }
                    else if(std::find(std::begin(nonTerminal), std::end(nonTerminal), nextNextSimpleToken)){
                        tokensTy toMerge = FIRSTset.at(startItemSet + " " + nextNextSimpleToken + " " + endItemSet);
                        FOLLOWset[nextExtendedToken].merge(toMerge);
                    }
                }
            }
        }
    }
    for(const auto &rule: extended){
        std::string lastTokenInRule = rule.second.back();
        std::string ruleName = rule.first;
        std::stringstream tokStream(lastTokenInRule);
        std::string lastToken;
        tokStream >> lastToken >> lastToken;
        std::string lastSimpleToken = lastToken.substr(0, lastToken.find('.'));
        if(std::find(std::begin(nonTerminal), std::end(nonTerminal), lastSimpleToken) != std::end(nonTerminal)){
            tokensTy toMerge = FOLLOWset[rule.first];
            FOLLOWset[lastTokenInRule].merge(toMerge);
        }
    }
    for(auto &item: FOLLOWset){
        for(const auto &token: item.second){
            if(token.size() == 0){
                continue;
            }
            if(std::isdigit(token.at(0))){
                if(FOLLOWset.find(token) != FOLLOWset.end()){
                    tokensTy toMerge = FOLLOWset.at(token);
                    item.second.merge(toMerge);
                }
            }
        }
    }
    return FOLLOWset;
}

static std::pair<std::string, grammarTy> getOriginalRule(std::string name, grammarTy extendedRule, const grammarListTy &grammarList){
    grammarTy result;
    for(const auto &s: extendedRule){
        std::stringstream tokStream(s);
        std::string nextToken, endStateNum;
        tokStream >> nextToken >> nextToken >> endStateNum;
        if(endStateNum != ""){
            result.push_back(nextToken);
        }
        else{
            result.push_back("");
        }
    }
    for(const auto &rule: grammarList){
        std::stringstream tokStream(name);
        std::string tokenName, endStateNum;
        tokStream >> tokenName >> tokenName >> endStateNum;
        std::string simpleName = rule.first.substr(0, rule.first.find('.'));
        if(rule.second == result && simpleName == tokenName){
            return rule;
        }
    }
    return std::make_pair("idk", grammarTy({"idk"}));
}

static tableTy generateActionGotoTable(const std::vector<itemSetTy> &itemSets, const itemSetTy &extended, const firstSetTy &FIRSTset, const firstSetTy &FOLLOWset, const grammarListTy &grammarList){
    tableTy result;
    result["eof"] = std::vector<std::string>(itemSets.size(), "");
    for(const auto &s: nonTerminal){
        result[s] = std::vector<std::string>(itemSets.size(), "");
    }
    for(const auto &s: tokenNames){
        result[s] = std::vector<std::string>(itemSets.size(), "");
    }
    for(int i = 0; i < transitionTable.size(); i++){
        for(const auto &toNum: transitionTable.at(i)){
            if(std::find(std::begin(nonTerminal), std::end(nonTerminal), toNum.first) != std::end(nonTerminal)){
                result[toNum.first].at(i) = std::to_string(toNum.second);
            }
            else if(std::find(std::begin(tokenNames), std::end(tokenNames), toNum.first) != std::end(tokenNames)){
                result[toNum.first].at(i) = "s" + std::to_string(toNum.second);
            }
        }
    }
    std::vector<std::tuple<std::string, grammarTy, tokensTy>> FOLLOWandExtended;
    for(auto rule: extended){
        FOLLOWandExtended.push_back(std::make_tuple(rule.first, rule.second, FOLLOWset.at(rule.first)));
    }
    std::vector<std::tuple<std::string, grammarTy, tokensTy>> actionTable(itemSets.size());
    for(auto it = FOLLOWandExtended.begin(); it != FOLLOWandExtended.end(); it++){
        auto rule = *it;
        auto originalRule = getOriginalRule(std::get<0>(rule), std::get<1>(rule), grammarList);
        std::stringstream last(std::get<1>(rule).back());
        std::string temp, lastS;
        last >> temp >> temp >> lastS;
        int lastSet;
        if(lastS == ""){
            lastSet = std::stoi(temp);
        }
        else{
            lastSet = std::stoi(lastS);
        }
        for(const auto &s: std::get<2>(rule)){
            result[s].at(lastSet) += " r" + originalRule.first;
        }
        for(auto checkIt = it + 1; checkIt != FOLLOWandExtended.end(); checkIt++){
            auto rule2 = *checkIt;
            auto originalRule2 = getOriginalRule(std::get<0>(rule2), std::get<1>(rule2), grammarList);
            std::stringstream last(std::get<1>(rule2).back());
            std::string temp, lastS;
            last >> temp >> temp >> lastS;
            int lastSet2;
            if(lastS == ""){
                lastSet2 = std::stoi(temp);
            }
            else{
                lastSet2 = std::stoi(lastS);
            }
            if(originalRule2.second == originalRule.second && lastSet == lastSet2){
                for(const auto &s: std::get<2>(rule2)){
                    result[s].at(lastSet) += " r" + originalRule2.first;
                    result[s].at(lastSet2) += " r" + originalRule2.first;
                }
                checkIt = FOLLOWandExtended.erase(checkIt);
                checkIt --;
            }
        }
    }
    for(int i = 1; i < itemSets.size(); i++){
        if(itemSets.at(i).front().first == "start"){
            result["eof"].at(i) = "accepted";
        }
    }
    return result;
}

static A_decList parseWithTable(L_tokenList list, tableTy table, const grammarListTy &grammarList){
    std::vector<int> stack;
    stack.push_back(0);
    std::string action;
    do{
        std::string tokenToRead = list.front() -> kind;
        // std::cout << list.front() -> kind << std::endl;
        action = table.at(list.front() -> kind).at(stack.back());
        // std::cout << "Stack now: ";
        // for(const auto &j: stack){
        //     std::cout << j << " ";
        // }
        // std::cout << std::endl;
        if(action.size() == 0){
            if(table.at("").at(stack.back()).size() > 0){
                action = table.at("").at(stack.back());
                tokenToRead = "";
            }
            else{
                L_errorPos errorPos = L_getErrorPos(list.front() -> start);
                std::cerr << "Unexpected " << list.front() -> kind << " " << errorPos.lineNum << std::endl;
                exit(0);
            }
        }
        if(action.at(0) == ' '){
            action = action.substr(1, action.size() - 1);
        }
        if(std::count(action.begin(), action.end(), 's') > 0){
            // std::cout << "conflict found " << action << std::endl;
        }
        action = action.substr(0, action.find(" "));
        if(list.front() -> kind == "id"){
            // std::cout << list.front() -> u.id << " ";
        }
        if(action.at(0) == 's'){
            int stackNum = std::stoi(action.substr(1, action.size() - 1));
            // std::cout << "stackNum " << stackNum << " " << list.front() -> kind << std::endl;
            stack.push_back(stackNum);
            if(tokenToRead == list.front() -> kind){
                list.pop_front();
            }
        }
        else if(action.at(0) == 'r'){
            std::string reduceRule = action.substr(1, action.size() - 1);
            int ruleSize = grammarList.at(reduceRule).size();
            // std::cout << "reduceRule " + reduceRule + " "<< ruleSize << std::endl;
            for(int ii = 0; ii < ruleSize; ii++){
                stack.pop_back();
            }
            reduceRule = reduceRule.substr(0, reduceRule.find("."));
            // std::cout << reduceRule << " " << stack.back() << " " << table.at(reduceRule).at(stack.back()) << std::endl;
            stack.push_back(std::stoi(table.at(reduceRule).at(stack.back())));
        }
        else if(action == "accepted"){
            std::cout << "parsing finished." << std::endl;
            break;
        }
        else{
            std::cout << action << " undefined" << std::endl;
        }
    } while(list.size() != 0);
    std::cout << "finished" << std::endl;
}

static grammarTy createGrammarList(std::string ruleStr, std::string ruleNameStr, grammarListTy &grammarList, json input){
    std::string tok;
    std::regex tokenWithName("([a-zA-Z0-9]+)\\(([a-zA-Z0-9]+)\\)");
    std::regex tokenWithPeriod("([a-zA-Z0-9]+)\\.([a-zA-Z0-9]+)");
    std::smatch match;
    std::deque<std::string> tokens;
    std::stringstream rule(ruleStr);
    while(rule >> tok){
        if(std::find(std::begin(tokenNames), std::end(tokenNames), tok) == std::end(tokenNames)){
            if(std::regex_search(tok, match, tokenWithName)){
                tokens.push_back(match[1]);
                continue;
            }
            if(std::regex_search(tok, match, tokenWithPeriod)){
                std::cout << match[0] << std::endl;
                std::string tempRuleStr = ruleStr;
                if(input[match[1]][match[2]].is_string()){
                    tempRuleStr = std::regex_replace(tempRuleStr, std::regex(tok), input[match[1]][match[2]].get<std::string>());
                    grammarList[ruleNameStr + "." + tok] = createGrammarList(tempRuleStr, ruleNameStr, grammarList, input);
                }
                if(input[match[1]][match[2]].is_object()){
                    for(const auto &item: input[match[1]][match[2]].items()){
                        tempRuleStr = ruleStr;
                        tempRuleStr = std::regex_replace(tempRuleStr, std::regex(tok), item.value().get<std::string>());
                        grammarList[ruleNameStr + "." + tok + "." + item.key()] = createGrammarList(tempRuleStr, ruleNameStr, grammarList, input);
                    }
                    return tokens;
                }
                continue;
            }
            if(std::find(std::begin(nonTerminal), std::end(nonTerminal), tok) != std::end(nonTerminal)){
                tokens.push_back(tok);
                continue;
            }
            if(grammarList.count(tok)){
                tokens.push_back(tok);
            }
            std::cerr << "Illegal token in grammar " << ruleNameStr << ": " << tok << std::endl;
        }
        tokens.push_back(tok);
    }
    if(tokens.size() == 0){
        tokens.push_back("");
    }
    return tokens;
}

A_decList P_parse(L_tokenList list, const char *filename1){
    std::ifstream jInput(filename1);
    json j;
    jInput >> j;
    grammarListTy grammarList;
    std::string tok;
    std::regex tokenWithName("([a-zA-Z0-9]+)\\(([a-zA-Z0-9]+)\\)");
    std::smatch match;
    grammarList["start"] = std::deque<std::string>({"declist"});
    for(const auto &item2: j["grammar"].items()){
        for(const auto &item: item2.value().items()){
            if(item.value().is_string()){
                grammarTy temp = createGrammarList(item.value().get<std::string>(), item2.key() + "." + item.key(), grammarList, j["grammar"]);
                if(temp.size() != 0){
                    grammarList[item2.key() + "." + item.key()] = temp;
                }
            }
            else if(item.value().is_object()){
                for(const auto &i: item.value().items()){
                    grammarTy temp = createGrammarList(i.value().get<std::string>(), item2.key() + "." + item.key() + "." + i.key(), grammarList, j["grammar"]);
                    if(temp.size() != 0){
                        grammarList[item2.key() + "." + item.key() + "." + i.key()] = temp;
                    }
                }
            }
        }
    }
    transitionTable.push_back(std::unordered_map<std::string, int>());
    std::vector<itemSetTy> itemSets = toItem0(grammarList);
    itemSets.front().push_back(std::make_pair("additionalRules", std::deque<std::string>({"------------------"})));
    itemSets = createAllItemSets(itemSets, grammarList);
    itemSetTy extended = toExtendedGrammar(itemSets);
    std::cout << itemSets.size() << std::endl;
    firstSetTy FIRSTset = generateFirstSet(extended);
    firstSetTy FOLLOWset = generateFollowSet(extended, FIRSTset);
    tableTy actionGotoTable = generateActionGotoTable(itemSets, extended, FIRSTset, FOLLOWset, grammarList);
    A_decList result = parseWithTable(list, actionGotoTable, grammarList);
    // debug grammarList
    // int a = 0;
    // for(const auto &s: grammarList){
    //     std::cout << a << " " << s.first << ": ";
    //     for(const auto &t: s.second){
    //         std::cout << t << " ";
    //     }
    //     std::cout << std::endl;
    //     a++;
    // }

    //debug itemSets

    // for(int i = 0; i < itemSets.size(); i++){
    //     std::cout << "itemSet" << i << ":" << std::endl;
    //     for(const auto &j: itemSets.at(i)){
    //         std::cout << j.first << "-> ";
    //         for(const auto &k: j.second){
    //             std::cout << k << " ";
    //         }
    //     std::cout << std::endl;
    //     }
    //     std::cout << std::endl;
    // }

    //debug transition table

    // for(const auto &tokenName: tokenNames){
    //     std::cout << tokenName << " ";
    // }
    // for(const auto &nonTerminals: nonTerminal){
    //     std::cout << nonTerminals << " ";
    // }
    // std::cout << std::endl;
    // unsigned num = 0;
    // for(auto &i: transitionTable){
    //     std::cout << num << ":  ";
    //     num++;
    //     for(const auto &tokenName: tokenNames){
    //         std::cout << " " << tokenName << " " << i[tokenName];
    //     }
    //     for(const auto &nonTerminals: nonTerminal){
    //         std::cout << " " << i[nonTerminals];
    //     }
    //     std::cout << std::endl;
    // }

    //debug extended grammar

    // for(const auto &i: extended){
    //     std::cout << i.first << " -----> ";
    //     for(const auto &j: i.second){
    //         std::cout << j << " ";
    //     }
    //     std::cout << std::endl;
    // }

    //debug FIRST set 

    // for(const auto &i: FIRSTset){
    //     std::cout << i.first << " ----------> ";
    //     for(const auto &j: i.second){
    //         std::cout << j << ", ";
    //     }
    //     std::cout << std::endl;
    // }

    //debug FOLLOW set

    // for(const auto &i: FOLLOWset){
    //     std::cout << i.first << " ----------> ";
    //     for(const auto &j: i.second){
    //         std::cout << j << ", ";
    //     }
    //     std::cout << std::endl;
    // }

    //debug action goto table
    // for(const auto &i: actionGotoTable){
    //     std::cout << i.first << ": ";
    //     int a = 0;
    //     for(const auto &j: i.second){
    //         std::cout << a << "(" << j << ") ";
    //         a++;
    //     }
    //     std::cout << std::endl;
    // }
    // return NULL;
}