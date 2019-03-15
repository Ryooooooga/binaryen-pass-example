# Binaryen を用いた WebAssembly カスタムパスの作成例

注意点:

1. `wasm::ModuleReader::read*()`はファイルが読めないと`exit(1)`する。

2. `wasm::ModuleReader::read*()`はファイルの形式が間違っていると`wasm::ParseException`を投げるがこれは`std::except`を継承していない。

3. `wasm::ModuleReader::read()/readText()`で読める WebAssembly テキストファイルは`.wast`形式であり、`.wat`形式とは互換性がない。
