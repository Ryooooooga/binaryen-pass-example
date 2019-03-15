(module
 (export "add" (func $add))
 (func $add (param $0 i32) (param $1 i32) (param $2 i32) (result i32)
  (i32.add
   (local.get $0)
   (i32.add
    (local.get $1)
    (local.get $2)))))
