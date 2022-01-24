#include "parser.hpp"

std::string _mainFuncName = "";
std::string _stringClassName = "";
std::string _arrayTemplateName = "";
std::string _counterName = "";

// static std::string tokenNames[] = {"char","string","hex","int","real",",",":",";","(",")","[","]","{","}",".","->","<-","=>","+","-","*","/","==","!=","<=","<",">=",">","&&","||","=","if","then","else","from","to","break","inttype","realtype","continue","return","type","void","nul","true","false","boolean","chartype","%","&","shorttype","function","loop","jsload","sizeof","class","private","public","protected","repeat","jsexport","id","uminus","lower_than_else", "eof", ""};
static std::string nonTerminal[] = {"exp", "var", "varExp", "dec", "stm", "ty", "field", "explist", "stmlist", "declist", "memlist", "fieldlist", "oper", "funcAndVar", "funcAndVarList", "mems"};

static std::deque<std::string> tokenNames;
static std::map<std::string, unsigned> operators;

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
            if(result[s].at(lastSet).find("r" + originalRule.first + " ") == std::string::npos){
                result[s].at(lastSet) += " r" + originalRule.first + " ";
            }
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
                    if(result[s].at(lastSet).find("r" + originalRule2.first + " ") == std::string::npos){
                        result[s].at(lastSet) += " r" + originalRule2.first + " ";
                    }
                    if(result[s].at(lastSet2).find("r" + originalRule2.first + " ") == std::string::npos){
                        result[s].at(lastSet2) += " r" + originalRule2.first + " ";
                    }
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

static L_token reduce(L_tokenList &list, std::string ruleName, const grammarListTy &grammarList, const grammarListTy &originalGrammarList){
    int ruleSize = grammarList.at(ruleName).size() - std::count(grammarList.at(ruleName).begin(), grammarList.at(ruleName).end(), "");
    // if the reduce rule is longer the current list
    if(ruleSize > list.size()){
        std::cout << ruleSize << "????" << list.size() << std::endl;
        exit(0);
    }
    //take out the part of the tokenlist that corresponds with the grammar rule.
    L_tokenList reduceList(list.end() - ruleSize, list.end());
    L_token result = std::make_shared<L_token_>();
    result -> kind = ruleName;
    if(reduceList.size() > 0){
        result -> start = reduceList.front() -> start;
        result -> end = reduceList.front() -> end;
    }
    else{
        result -> start = list.back() -> start;
        result -> end = list.back() -> end;
    }
    std::map<std::string, L_tokenVal> tokenData;
    for(int i = 0; i < reduceList.size(); i++){
        tokenData[originalGrammarList.at(ruleName).at(i)] = reduceList.at(i) -> u;
        // std::cout << originalGrammarList.at(ruleName).at(i) << " <-> " << reduceList.at(i) -> kind << " ";
    }
    //exp
    {
        if(ruleName == "exp.op"){
            result -> u.exp = A_OpExp(result -> start, tokenData.at("oper").oper, tokenData.at("exp(1)").exp, tokenData.at("exp(2)").exp);
        }
        else if(ruleName == "exp.minus"){
            result -> u.exp = A_OpExp(result -> start, A_minusOp, A_IntExp(0, 0), tokenData.at("exp").exp);
        }
        else if(ruleName == "exp.paren"){
            result -> u.exp = A_ParenExp(result -> start, tokenData.at("exp").exp);
        }
        else if(ruleName == "exp.call"){
            result -> u.exp = A_CallExp(result -> start, tokenData.at("exp").exp, tokenData.at("explist").expList);
        }
        else if(ruleName == "exp.address"){
            result -> u.exp = A_AddressExp(result -> start, tokenData.at("var").var);
        }
        else if(ruleName == "exp.sizeof"){
            result -> u.exp = A_SizeofExp(result -> start, tokenData.at("var").var);
        }
        else if(ruleName == "exp.array"){
            result -> u.exp = A_ArrayExp(result -> start, tokenData.at("explist").expList);
        }
        else if(ruleName == "exp.func"){
            result -> u.exp = A_FuncExp(result -> start, tokenData.at("fieldlist(params)").fieldList, tokenData.at("fieldlist(result)").fieldList, tokenData.at("stm").stm);
        }
        else if(ruleName == "exp.var"){
            result -> u.exp = A_VarExp(result -> start, tokenData.at("var").var);
        }
        else if(ruleName == "exp.field"){
            result -> u.exp = A_FieldExp(result -> start, tokenData.at("exp").exp, S_Symbol(tokenData.at("id").id));
        }
        else if(ruleName == "exp.subscript"){
            result -> u.exp = A_SubscriptExp(result -> start, tokenData.at("exp(arrayname)").exp, tokenData.at("exp(index)").exp);
        }
        else if(ruleName == "exp.arrowfield"){
            result -> u.exp = A_ArrowFieldExp(result -> start, tokenData.at("exp").exp, S_Symbol(tokenData.at("id").id));
        }
        else if(ruleName == "exp.int"){
            result -> u.exp = A_IntExp(result -> start, tokenData.at("int").intt);
        }
        else if(ruleName == "exp.char"){
            result -> u.exp = A_CharExp(result -> start, tokenData.at("char").charr);
        }
        else if(ruleName == "exp.string"){
            result -> u.exp = A_StringExp(result -> start, tokenData.at("string").stringg);
        }
        else if(ruleName == "exp.real"){
            result -> u.exp = A_RealExp(result -> start, tokenData.at("real").real);
        }
        else if(ruleName == "exp.true"){
            result -> u.exp = A_BoolExp(EM_tokPos, TRUE);
        }
        else if(ruleName == "exp.false"){
            result -> u.exp = A_BoolExp(EM_tokPos, FALSE);
        }
        else if(ruleName == "exp.typeeq"){
            result -> u.exp = A_TypeEqExp(EM_tokPos, tokenData.at("ty(1)").type, tokenData.at("ty(2)").type);
        }
    }
    //oper
    {
        if(ruleName == "oper.add"){
            result -> u.oper = A_plusOp;
        }
        else if(ruleName == "oper.sub"){
            result -> u.oper = A_minusOp;
        }
        else if(ruleName == "oper.mul"){
            result -> u.oper = A_timesOp;
        }
        else if(ruleName == "oper.div"){
            result -> u.oper = A_divideOp;
        }
        else if(ruleName == "oper.mod"){
            result -> u.oper = A_modOp;
        }
        else if(ruleName == "oper.eq"){
            result -> u.oper = A_eqOp;
        }
        else if(ruleName == "oper.neq"){
            result -> u.oper = A_neqOp;
        }
        else if(ruleName == "oper.ge"){
            result -> u.oper = A_geOp;
        }
        else if(ruleName == "oper.gt"){
            result -> u.oper = A_gtOp;
        }
        else if(ruleName == "oper.le"){
            result -> u.oper = A_leOp;
        }
        else if(ruleName == "oper.lt"){
            result -> u.oper = A_ltOp;
        }
        else if(ruleName == "oper.and"){
            result -> u.oper = A_andOp;
        }
        else if(ruleName == "oper.or"){
            result -> u.oper = A_orOp;
        }
    }
    //stm
    {
        if(ruleName == "stm.assign.normal"){
            result -> u.stm = A_AssignStm(result -> start, tokenData.at("var").var, tokenData.at("exp").exp, FALSE);
        }
        else if(ruleName == "stm.assign.add"){
            result -> u.stm = A_AssignStm(result -> start, tokenData.at("var").var, A_OpExp(result -> start, A_plusOp, A_VarExp(result -> start, tokenData.at("var").var), tokenData.at("exp").exp), FALSE);
        }
        else if(ruleName == "stm.assign.sub"){
            result -> u.stm = A_AssignStm(result -> start, tokenData.at("var").var, A_OpExp(result -> start, A_minusOp, A_VarExp(result -> start, tokenData.at("var").var), tokenData.at("exp").exp), FALSE);
        }
        else if(ruleName == "stm.assign.mul"){
            result -> u.stm = A_AssignStm(result -> start, tokenData.at("var").var, A_OpExp(result -> start, A_timesOp, A_VarExp(result -> start, tokenData.at("var").var), tokenData.at("exp").exp), FALSE);
        }
        else if(ruleName == "stm.assign.div"){
            result -> u.stm = A_AssignStm(result -> start, tokenData.at("var").var, A_OpExp(result -> start, A_divideOp, A_VarExp(result -> start, tokenData.at("var").var), tokenData.at("exp").exp), FALSE);
        }
        else if(ruleName == "stm.assign.increment"){
            result -> u.stm = A_AssignStm(result -> start, tokenData.at("var").var, A_OpExp(result -> start, A_plusOp, A_VarExp(result -> start, tokenData.at("var").var), A_IntExp(result -> start, 1)), FALSE);
        }
        else if(ruleName == "stm.assign.decrement"){
            result -> u.stm = A_AssignStm(result -> start, tokenData.at("var").var, A_OpExp(result -> start, A_minusOp, A_VarExp(result -> start, tokenData.at("var").var), A_IntExp(result -> start, 1)), FALSE);
        }
        else if(ruleName == "stm.if.if"){
            result -> u.stm = A_IfStm(result -> start, tokenData.at("exp").exp, tokenData.at("stm(then)").stm, NULL);
        }
        else if(ruleName == "stm.if.ifelse"){
            result -> u.stm = A_IfStm(result -> start, tokenData.at("exp").exp, tokenData.at("stm(then)").stm, tokenData.at("stm(else)").stm);
        }
        else if(ruleName == "stm.while"){
            result -> u.stm = A_WhileStm(result -> start, tokenData.at("exp").exp, tokenData.at("stm").stm);
        }
        else if(ruleName == "stm.dec"){
            result -> u.stm = A_DeclarationStm(result -> start, tokenData.at("dec(stm)").dec);
        }
        else if(ruleName == "stm.for"){
            result -> u.stm = A_ForStm(result -> start, tokenData.at("stm(init)").stm, tokenData.at("exp").exp, tokenData.at("stm(incr)").stm, tokenData.at("stm").stm);
        }
        else if(ruleName == "stm.return.noexp"){
            result -> u.stm = A_ReturnStm(result -> start, NULL);
        }
        else if(ruleName == "stm.return.exp"){
            result -> u.stm = A_ReturnStm(result -> start, tokenData.at("exp").exp);
        }
        else if(ruleName == "stm.return.var"){
            result -> u.stm = A_ReturnStm(result -> start, A_VarExp(result -> start, tokenData.at("var").var));
        }
        else if(ruleName == "stm.call.normal"){
            result -> u.stm = A_CallStm(result -> start, tokenData.at("exp").exp, tokenData.at("explist").expList);
        }
        else if(ruleName == "stm.call.noexp"){
            result -> u.stm = A_CallStm(result -> start, tokenData.at("exp").exp, A_ExpList(NULL, NULL));
        }
        else if(ruleName == "stm.loop"){
            // std::cout << "looooooooooooooop" << std::endl;
            result -> u.stm = A_LoopStm(result -> start, tokenData.at("stm").stm);
            // result -> u.stm = A_DeclarationStm(result -> start, A_FunctionDec(result -> start, A_FundecList(A_Fundec(result -> start, S_Symbol("loop"), A_FieldList(NULL, NULL), A_FieldList(NULL, NULL), tokenData.at("stm").stm), NULL)));
        }
        else if(ruleName == "stm.break"){
            result -> u.stm = A_BreakStm(result -> start);
        }
        else if(ruleName == "stm.compound"){
            result ->u.stm = A_CompoundStm(result -> start, tokenData.at("stmlist").stmList);
        }
        else if(ruleName == "stm.continue"){
            result -> u.stm = A_ContinueStm(result -> start);
        }
        else if(ruleName == "stm.repeat"){
            result -> u.stm = A_RepeatStm(EM_tokPos, tokenData.at("exp").exp, tokenData.at("stm").stm);
            // result -> u.stm = A_ForStm(EM_tokPos, A_DeclarationStm(EM_tokPos, A_VarDec(EM_tokPos, A_AssignStm(EM_tokPos, A_SimpleVar(EM_tokPos, S_Symbol("カウンタ")), A_IntExp(EM_tokPos, 0), TRUE), A_NameTy(EM_tokPos, S_Symbol("int")))), A_OpExp(EM_tokPos, A_eqOp, A_VarExp(EM_tokPos, A_SimpleVar(EM_tokPos, S_Symbol("カウンタ"))), tokenData.at("exp").exp), A_AssignStm(EM_tokPos, A_SimpleVar(EM_tokPos, S_Symbol("カウンタ")), A_OpExp(EM_tokPos, A_plusOp, A_VarExp(EM_tokPos, A_SimpleVar(EM_tokPos, S_Symbol("カウンタ"))), A_IntExp(EM_tokPos, 1)), FALSE), tokenData.at("stm").stm);
        }
    }
    //dec
    {
        if(ruleName == "dec.var.init"){
            result -> u.dec = A_VarDec(result -> start, A_AssignStm(result -> start, tokenData.at("var").var, tokenData.at("exp").exp, TRUE), tokenData.at("ty").type);
        }
        else if(ruleName == "dec.var.noinit"){
            result -> u.dec = A_VarDec(result -> start, A_AssignStm(result -> start, tokenData.at("var").var, NULL, TRUE), tokenData.at("ty").type);
        }
        else if(ruleName == "dec.object"){
            result -> u.dec = A_ObjectDec(result -> start, tokenData.at("ty").type, tokenData.at("var").var -> u.simple, tokenData.at("explist").expList);
        }
        else if(ruleName == "dec.class.noinherit"){
            result -> u.dec = A_ClassDec(result -> start, S_Symbol(tokenData.at("id(name)").id), tokenData.at("memlist").memList, NULL);
        }
        else if(ruleName == "dec.class.inherit"){
            result -> u.dec = A_ClassDec(result -> start, S_Symbol(tokenData.at("id(name)").id), tokenData.at("memlist").memList, S_SymbolList(S_Symbol(tokenData.at("id(inherit)").id), NULL));
        }
        else if(ruleName == "dec.func.normal"){
            result -> u.dec = A_FunctionDec(result -> start, A_FundecList(A_Fundec(result -> start, S_Symbol(tokenData.at("id").id), tokenData.at("fieldlist(params)").fieldList, tokenData.at("fieldlist(result)").fieldList, tokenData.at("stm").stm), NULL));
        }
        else if(ruleName == "dec.func.noparam"){
            result -> u.dec = A_FunctionDec(result -> start, A_FundecList(A_Fundec(result -> start, S_Symbol(tokenData.at("id").id), A_FieldList(NULL, NULL), tokenData.at("fieldlist(result)").fieldList, tokenData.at("stm").stm), NULL));
        }
        else if(ruleName == "dec.func.noresult"){
            result -> u.dec = A_FunctionDec(result -> start, A_FundecList(A_Fundec(result -> start, S_Symbol(tokenData.at("id").id), tokenData.at("fieldlist(params)").fieldList, A_FieldList(NULL, NULL), tokenData.at("stm").stm), NULL));
        }
        else if(ruleName == "dec.func.noparamnoresult"){
            result -> u.dec = A_FunctionDec(result -> start, A_FundecList(A_Fundec(result -> start, S_Symbol(tokenData.at("id").id), A_FieldList(NULL, NULL), A_FieldList(NULL, NULL), tokenData.at("stm").stm), NULL));
        }
        else if(ruleName == "dec.jsload.normal"){
            result -> u.dec = A_FuncImport(result -> start, S_Symbol(tokenData.at("id").id), tokenData.at("fieldlist(params)").fieldList, tokenData.at("fieldlist(result)").fieldList, tokenData.at("string(module)").stringg, tokenData.at("string(func)").stringg);
        }
        else if(ruleName == "dec.jsload.noparam"){
            result -> u.dec = A_FuncImport(result -> start, S_Symbol(tokenData.at("id").id), A_FieldList(NULL, NULL), tokenData.at("fieldlist(result)").fieldList, tokenData.at("string(module)").stringg, tokenData.at("string(func)").stringg);
        }
        else if(ruleName == "dec.jsload.noresult"){
            result -> u.dec = A_FuncImport(result -> start, S_Symbol(tokenData.at("id").id), tokenData.at("fieldlist(params)").fieldList, A_FieldList(NULL, NULL), tokenData.at("string(module)").stringg, tokenData.at("string(func)").stringg);
        }
        else if(ruleName == "dec.jsload.noparamnoresult"){
            result -> u.dec = A_FuncImport(result -> start, S_Symbol(tokenData.at("id").id), A_FieldList(NULL, NULL), A_FieldList(NULL, NULL), tokenData.at("string(module)").stringg, tokenData.at("string(func)").stringg);
        }
        else if(ruleName == "dec.jsexport"){
            result -> u.dec = A_FuncExport(result -> start, S_Symbol(tokenData.at("id").id), tokenData.at("string").stringg);
        }
        else if(ruleName == "dec.template"){
            result -> u.dec = A_TemplateDec(result -> start, S_Symbol(tokenData.at("id").id), tokenData.at("dec").dec);
        }
        else if(ruleName == "dec.operator.normal"){
            result -> u.dec = A_OperatorDec(result -> start, tokenData.at("oper").oper, tokenData.at("fieldlist(params)").fieldList, tokenData.at("fieldlist(result)").fieldList, tokenData.at("stm").stm);
        }
        else if(ruleName == "dec.operator.noparam"){
            result -> u.dec = A_OperatorDec(result -> start, tokenData.at("oper").oper, A_FieldList(NULL, NULL), tokenData.at("fieldlist(result)").fieldList, tokenData.at("stm").stm);
        }
        else if(ruleName == "dec.operator.noresult"){
            result -> u.dec = A_OperatorDec(result -> start, tokenData.at("oper").oper, tokenData.at("fieldlist(params)").fieldList, A_FieldList(NULL, NULL), tokenData.at("stm").stm);
        }
        else if(ruleName == "dec.operator.noparamnoresult"){
            result -> u.dec = A_OperatorDec(result -> start, tokenData.at("oper").oper, A_FieldList(NULL, NULL), A_FieldList(NULL, NULL), tokenData.at("stm").stm);
        }
        else if(ruleName == "dec.operator.assignnormal"){
            result -> u.dec = A_OperatorDec(result -> start, A_assignOp, tokenData.at("fieldlist(params)").fieldList, tokenData.at("fieldlist(result)").fieldList, tokenData.at("stm").stm);
        }
        else if(ruleName == "dec.operator.assignnoparam"){
            result -> u.dec = A_OperatorDec(result -> start, A_assignOp, A_FieldList(NULL, NULL), tokenData.at("fieldlist(result)").fieldList, tokenData.at("stm").stm);
        }
        else if(ruleName == "dec.operator.assignnoresult"){
            result -> u.dec = A_OperatorDec(result -> start, A_assignOp, tokenData.at("fieldlist(params)").fieldList, A_FieldList(NULL, NULL), tokenData.at("stm").stm);
        }
        else if(ruleName == "dec.operator.assignnoparamnoresult"){
            result -> u.dec = A_OperatorDec(result -> start, A_assignOp, A_FieldList(NULL, NULL), A_FieldList(NULL, NULL), tokenData.at("stm").stm);
        }
    }
    //var
    {
        if(ruleName == "var.simple"){
            result -> u.var = A_SimpleVar(result -> start, S_Symbol(tokenData.at("id").id));
        }
        else if(ruleName == "var.field"){
            result -> u.var = A_FieldVar(result -> start, tokenData.at("var").var, S_Symbol(tokenData.at("id").id));
        }
        else if(ruleName == "var.deref"){
            result -> u.var = A_DerefVar(result -> start, tokenData.at("var").var);
        }
        else if(ruleName == "var.subscript"){
            result -> u.var = A_SubscriptVar(result -> start, tokenData.at("var").var, tokenData.at("exp").exp);
        }
        else if(ruleName == "var.arrowfield"){
            result -> u.var = A_ArrowFieldVar(result -> start, tokenData.at("var").var, S_Symbol(tokenData.at("id").id)); 
        }
    }
    //varExp
    {
        if(ruleName == "varExp.var"){
            result -> u.exp = A_VarExp(result -> start, tokenData.at("var").var);
        }
        else if(ruleName == "varExp.field"){
            result -> u.exp = A_FieldExp(result -> start, tokenData.at("exp").exp, S_Symbol(tokenData.at("id").id));
        }
        else if(ruleName == "varExp.subscript"){
            result -> u.exp = A_SubscriptExp(result -> start, tokenData.at("exp(arrayname)").exp, tokenData.at("exp(index)").exp);
        }
        else if(ruleName == "varExp.arrowfield"){
            result -> u.exp = A_ArrowFieldExp(result -> start, tokenData.at("exp").exp, S_Symbol(tokenData.at("id").id));
        }
    }
    //ty
    {
        if(ruleName == "ty.name"){
            result -> u.type = A_NameTy(result -> start, S_Symbol(tokenData.at("id").id));
        }
        else if(ruleName == "ty.void"){
            result -> u.type = A_NameTy(result -> start, S_Symbol("void"));
        }
        else if(ruleName == "ty.int"){
            result -> u.type = A_NameTy(result -> start, S_Symbol("int"));
        }
        else if(ruleName == "ty.real"){
            result -> u.type = A_NameTy(result -> start, S_Symbol("real"));
        }
        else if(ruleName == "ty.bool"){
            result -> u.type = A_NameTy(result -> start, S_Symbol("bool"));
        }
        else if(ruleName == "ty.char"){
            result -> u.type = A_NameTy(result -> start, S_Symbol("char"));
        }
        else if(ruleName == "ty.short"){
            result -> u.type = A_NameTy(result -> start, S_Symbol("short"));
        }
        else if(ruleName == "ty.pointer"){
            result -> u.type = A_PointerTy(result -> start, tokenData.at("ty").type);
        }
        else if(ruleName == "ty.poly"){
            result -> u.type = A_PolyTy(result -> start, S_Symbol(tokenData.at("id").id), tokenData.at("ty").type);
        }
        else if(ruleName == "ty.funcnormal"){
            result -> u.type = A_FuncTy(result -> start, tokenData.at("fieldlist(params)").fieldList, tokenData.at("fieldlist(result)").fieldList);
        }
        else if(ruleName == "ty.funcnoparam"){
            result -> u.type = A_FuncTy(result -> start, A_FieldList(NULL, NULL), tokenData.at("fieldlist(result)").fieldList);
        }
        else if(ruleName == "ty.funcnoresult"){
            result -> u.type = A_FuncTy(result -> start, tokenData.at("fieldlist(params)").fieldList, A_FieldList(NULL, NULL));
        }
        else if(ruleName == "ty.funcnoparamnoresult"){
            result -> u.type = A_FuncTy(result -> start, A_FieldList(NULL, NULL), A_FieldList(NULL, NULL));
        }
    }
    //mems
    {
        if(ruleName == "mems.nospecifier"){
            result -> u.memList = A_ClassMemFromDecList(tokenData.at("declist").decList, A_public);
        }
        else if(ruleName == "mems.public"){
            result -> u.memList = A_ClassMemFromDecList(tokenData.at("declist").decList, A_public);
        }
        else if(ruleName == "mems.private"){
            result -> u.memList = A_ClassMemFromDecList(tokenData.at("declist").decList, A_private);
        }
        else if(ruleName == "mems.protected"){
            result -> u.memList = A_ClassMemFromDecList(tokenData.at("declist").decList, A_protected);
        }
    }
    //memlist
    {
        if(ruleName == "memlist.mems"){
            result -> u.memList = tokenData.at("mems").memList;
        }
        else if(ruleName == "memlist.memlist"){
            result -> u.memList = A_ClassMemFromTwoList(tokenData.at("memlist").memList, tokenData.at("mems").memList);
        }
    }
    //field
    {
        if(ruleName == "field.noinit"){
            result -> u.field = A_Field(result -> start, tokenData.at("var").var, tokenData.at("ty").type);
        }
        else if(ruleName == "field.func"){
            result -> u.field = A_Field(result -> start, A_SimpleVar(result -> start, S_Symbol(tokenData.at("id").id)), A_FuncTy(result -> start, tokenData.at("fieldlist(params)").fieldList, tokenData.at("fieldlist(result)").fieldList));
        }
    }
    //explist
    {
        if(ruleName == "explist.null"){
            result -> u.expList = A_ExpList(NULL, NULL);
        }
        else if(ruleName == "explist.exp"){
            result -> u.expList = A_ExpList(tokenData.at("exp").exp, NULL);
        }
        else if(ruleName == "explist.explist"){
            result -> u.expList = A_ExpList(tokenData.at("exp").exp, tokenData.at("explist").expList);
        }
    }
    //stmlist
    {
        if(ruleName == "stmlist.null"){
            result -> u.stmList = A_StmList(NULL, NULL);
        }
        else if(ruleName == "stmlist.stm"){
            result -> u.stmList = A_StmList(tokenData.at("stm").stm, NULL);
        }
        else if(ruleName == "stmlist.stmlist"){
            result -> u.stmList = A_StmList(tokenData.at("stm").stm, tokenData.at("stmlist").stmList);
        }
    }
    //declist
    {
        if(ruleName == "declist.null"){
            result -> u.decList = A_DecList(NULL, NULL);
        }
        else if(ruleName == "declist.dec"){
            result -> u.decList = A_DecList(tokenData.at("dec").dec, NULL);
        }
        else if(ruleName == "declist.declist"){
            result -> u.decList = A_DecList(tokenData.at("dec").dec, tokenData.at("declist").decList);
        }
    }
    //fieldlist
    {
        if(ruleName == "fieldlist.null"){
            result -> u.fieldList = A_FieldList(NULL, NULL);
        }
        else if(ruleName == "fieldlist.field"){
            result -> u.fieldList = A_FieldList(tokenData.at("field").field, NULL);
        }
        else if(ruleName == "fieldlist.fieldlist"){
            result -> u.fieldList = A_FieldList(tokenData.at("field").field, tokenData.at("fieldlist").fieldList);
        }
    }
    for(int i = 0; i < ruleSize; i++){
        list.pop_back();
    }
    return result;
}

static A_decList parseWithTable(L_tokenList list, tableTy table, const grammarListTy &grammarList, const grammarListTy &originalGrammarList){
    std::vector<int> stack;
    L_tokenList resultList;
    stack.push_back(0);
    std::string action;
    do{
        std::string tokenToRead = list.front() -> kind;
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
                // L_errorPos errorPos = L_getErrorPos(list.front() -> start);
                std::string token = list.front() -> kind;
                if(token == "id"){
                    token = list.front() -> u.id;
                }
                else if(token == "char"){
                    token = "\'" + std::string(list.front() -> u.charr) + "\'";
                }
                else if(token == "string"){
                    token = "\"" + std::string(list.front() -> u.stringg) + "\"";
                }
                else if(token == "real"){
                    token = std::to_string(list.front() -> u.real);
                }
                else if(token == "int"){
                    token = std::to_string(list.front() -> u.intt);
                }
                // std::cerr << "Unexpected " << list.front() -> kind << " on line " << errorPos.lineNum << " and column " << errorPos.columnNum << std::endl;
                std::cout << list.front() -> start << std::endl;
                EM_error(list.front() -> start, "parser.unexpectedtoken %s", token.c_str());
                exit(0);
            }
        }
        if(action.at(0) == ' '){
            action = action.substr(1, action.size() - 1);
        }
        if(std::count(action.begin(), action.end(), 's') > 0){
            // std::cout << "conflict found " << action << std::endl;
        }
        
        std::vector<std::string> actions;
        std::string order = "";
        std::stringstream actionStream(action);
        while(actionStream >> order){
            actions.push_back(order);
        }
        action = actions.at(0);
        if(actions.size() > 1){
            if(operators[tokenToRead] && actions.at(1) == "rexp.op"){
                if(resultList.size() > 1){
                    // std::cout << operators[grammarList.at(resultList.at(resultList.size() - 2) -> kind).at(0)] << " ";
                    // std::cout << operators[tokenToRead] << " " << action << std::endl;
                    if(operators[grammarList.at(resultList.at(resultList.size() - 2) -> kind).at(0)] < operators[tokenToRead]){
                        action = actions.at(1);
                    }
                    else if(operators[grammarList.at(resultList.at(resultList.size() - 2) -> kind).at(0)] == operators[tokenToRead]){
                        action = actions.at(1);
                    }
                }
            }
        }
        // else{
        //     action = action.substr(0, action.find(" "));
        // }
        if(list.front() -> kind == "id"){
            // std::cout << list.front() -> u.id << " ";
        }
        
        if(action.at(0) == 's'){
            int stackNum = std::stoi(action.substr(1, action.size() - 1));
            // std::cout << "stackNum " << stackNum << " " << list.front() -> kind << std::endl;
            stack.push_back(stackNum);
            if(tokenToRead == list.front() -> kind){
                resultList.push_back(list.front());
                list.pop_front();
            }
        }
        else if(action.at(0) == 'r'){
            std::string reduceRule = action.substr(1, action.size() - 1);
            int ruleSize = grammarList.at(reduceRule).size();
            // std::cout << "reduceRule " + reduceRule + " "<< ruleSize << std::endl;
            L_token resultToken = reduce(resultList, reduceRule, grammarList, originalGrammarList);
            for(int ii = 0; ii < ruleSize; ii++){
                stack.pop_back();
            }
            resultList.push_back(resultToken);
            reduceRule = reduceRule.substr(0, reduceRule.find("."));
            // std::cout << reduceRule << " " << stack.back() << " " << table.at(reduceRule).at(stack.back()) << std::endl;
            stack.push_back(std::stoi(table.at(reduceRule).at(stack.back())));
        }
        else if(action == "accepted"){
            return resultList.back() -> u.decList;
            break;
        }
        else{
            std::cout << action << " undefined" << std::endl;
        }
    } while(list.size() != 0);
    return NULL;
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
                // std::cout << match[0] << std::endl;
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

static grammarTy createOriginalGrammarList(std::string ruleStr, std::string ruleNameStr, grammarListTy &grammarList, json input){
    std::string tok;
    std::regex tokenWithName("([a-zA-Z0-9]+)\\(([a-zA-Z0-9]+)\\)");
    std::regex tokenWithPeriod("([a-zA-Z0-9]+)\\.([a-zA-Z0-9]+)");
    std::smatch match;
    std::deque<std::string> tokens;
    std::stringstream rule(ruleStr);
    while(rule >> tok){
        if(std::find(std::begin(tokenNames), std::end(tokenNames), tok) == std::end(tokenNames)){
            if(std::regex_search(tok, match, tokenWithName)){
                tokens.push_back(match[0]);
                continue;
            }
            if(std::regex_search(tok, match, tokenWithPeriod)){
                // std::cout << match[0] << std::endl;
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

tableTy P_generateParseTable(const grammarListTy &grammarList){
    transitionTable.push_back(std::unordered_map<std::string, int>());
    std::vector<itemSetTy> itemSets = toItem0(grammarList);
    itemSets.front().push_back(std::make_pair("additionalRules", std::deque<std::string>({"------------------"})));
    itemSets = createAllItemSets(itemSets, grammarList);
    itemSetTy extended = toExtendedGrammar(itemSets);
    for(int i = 0; i < itemSets.size(); i++){
        std::cout << "itemSet" << i << ":" << std::endl;
        for(const auto &j: itemSets.at(i)){
            std::cout << j.first << "-> ";
            for(const auto &k: j.second){
                std::cout << k << " ";
            }
        std::cout << std::endl;
        }
        std::cout << std::endl;
    }
    // std::cout << itemSets.size() << std::endl;
    firstSetTy FIRSTset = generateFirstSet(extended);
    firstSetTy FOLLOWset = generateFollowSet(extended, FIRSTset);
    return generateActionGotoTable(itemSets, extended, FIRSTset, FOLLOWset, grammarList);
}

void P_generateGrammarList(const std::string inputFname, grammarListTy &grammarList, grammarListTy &originalGrammarList){
    std::ifstream jInput(inputFname);
    json j;
    jInput >> j;
    _mainFuncName = j["tokens"]["main"].get<std::string>();
    _stringClassName = j["tokens"]["stringClass"].get<std::string>();
    _arrayTemplateName = j["tokens"]["arrayTemplate"].get<std::string>();
    _counterName = j["tokens"]["counter"].get<std::string>();
    
    std::string tok;
    std::regex tokenWithName("([a-zA-Z0-9]+)\\(([a-zA-Z0-9]+)\\)");
    std::smatch match;
    tokenNames = L_getTokenNames();
    grammarList["start"] = std::deque<std::string>({"declist"});
    originalGrammarList["start"] = std::deque<std::string>({"declist"});
    for(const auto &item2: j["grammar"].items()){
        for(const auto &item: item2.value().items()){
            if(item.value().is_string()){
                grammarTy temp = createGrammarList(item.value().get<std::string>(), item2.key() + "." + item.key(), grammarList, j["grammar"]);
                grammarTy temp2 = createOriginalGrammarList(item.value().get<std::string>(), item2.key() + "." + item.key(), originalGrammarList, j["grammar"]);
                if(temp.size() != 0){
                    grammarList[item2.key() + "." + item.key()] = temp;
                    originalGrammarList[item2.key() + "." + item.key()] = temp2;
                }
            }
            else if(item.value().is_object()){
                for(const auto &i: item.value().items()){
                    grammarTy temp = createGrammarList(i.value().get<std::string>(), item2.key() + "." + item.key() + "." + i.key(), grammarList, j["grammar"]);
                    grammarTy temp2 = createOriginalGrammarList(i.value().get<std::string>(), item2.key() + "." + item.key() + "." + i.key(), originalGrammarList, j["grammar"]);
                    if(temp.size() != 0){
                        grammarList[item2.key() + "." + item.key() + "." + i.key()] = temp;
                        originalGrammarList[item2.key() + "." + item.key() + "." + i.key()] = temp2;
                    }
                }
            }
        }
    }
}

void P_generateParseFile(const std::string inputFname, const std::string outputFname){
    grammarListTy grammarList;
    grammarListTy originalGrammarList;

    P_generateGrammarList(inputFname, grammarList, originalGrammarList);
    tableTy actionGotoTable = P_generateParseTable(grammarList);

    std::ofstream parseFile(outputFname);
    for(const auto &i: actionGotoTable){
        parseFile << i.first << " : ";
        int a = 0;
        for(const auto &j: i.second){
            parseFile << j << ":";
            // parseFile << a << "(" << j << ") ";
            a++;
        }
        parseFile << std::endl;
    }
}

static tableTy parseTableFromFile(const std::string parserFile){
    tableTy actionGotoTable;
    std::ifstream inputFile(parserFile);
    std::string line = "";
    while(std::getline(inputFile, line)){
        std::string tokenName = line.substr(0, line.find(' '));
        line = line.substr(line.find(' ') + 3);

        actionGotoTable[tokenName] = std::vector<std::string>();
        std::stringstream lineStream(line);
        std::string command = "";
        unsigned int tokNum = 0;
        while(std::getline(lineStream, command, ':')){
            actionGotoTable[tokenName].push_back(command);
        }
    }
    return actionGotoTable;
}

A_decList P_parse(L_tokenList list, const char *filename1){
    operators = L_getOperators();
    grammarListTy grammarList;
    grammarListTy originalGrammarList;

    P_generateGrammarList(filename1, grammarList, originalGrammarList);
    tableTy actionGotoTable = P_generateParseTable(grammarList);
    A_decList result = parseWithTable(list, actionGotoTable, grammarList, originalGrammarList);
    return result;
    {

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
}

A_decList P_parseWithFile(L_tokenList list, const std::string filename1, const std::string parserFile){
    operators = L_getOperators();
    // std::cout << "aaaaa" << std::endl;
    grammarListTy grammarList;
    grammarListTy originalGrammarList;

    P_generateGrammarList(filename1, grammarList, originalGrammarList);
    tableTy actionGotoTable = parseTableFromFile(parserFile);
    A_decList result = parseWithTable(list, actionGotoTable, grammarList, originalGrammarList);
    return result;
}