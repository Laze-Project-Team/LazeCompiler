(module (import "js" "mem" (memory 17))(import "console" "log" (func (param f64)))(import "webgl" "viewport" (func (param i64)(param i64)))(func (call 0(f64.convert_i64_s(i64.const 1))))(export "main" )(func $memorySize (result i32) (return (i32.const 0))) (export "memorySize"(func $memorySize)))