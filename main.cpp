#include <iostream>

#include <wasm-io.h>

struct CounterVisitor : public wasm::OverriddenVisitor<CounterVisitor, int> {
    int visitBlock(wasm::Block *curr) {
        int n = 0;

        for (const auto &expr : curr->list) {
            n += visit(expr);
        }

        return n;
    }
    int visitIf(wasm::If *curr) {
        int n = 0;

        n += visit(curr->condition);
        n += visit(curr->ifTrue);
        if (curr->ifFalse) n += visit(curr->ifFalse);

        return n;
    }
    int visitLoop(wasm::Loop *curr) {
        return visit(curr->body);
    }
    int visitBreak(wasm::Break *curr) {
        int n = 0;

        if (curr->condition) n += visit(curr->condition);
        if (curr->value) n += visit(curr->value);

        return n;
    }
    int visitSwitch(wasm::Switch *curr) {
        int n = 0;

        n += visit(curr->condition);
        if (curr->value) n += visit(curr->value);

        return n;
    }
    int visitCall(wasm::Call *curr) {
        int n = 0;

        for (const auto &expr : curr->operands) {
            n += visit(expr);
        }

        return n;
    }
    int visitCallIndirect(wasm::CallIndirect *curr) {
        int n = 0;

        n += visit(curr->target);

        for (const auto &expr : curr->operands) {
            n += visit(expr);
        }

        return n;
    }
    int visitGetLocal([[maybe_unused]] wasm::GetLocal *curr) {
        return 1;
    }
    int visitSetLocal(wasm::SetLocal *curr) {
        return visit(curr->value);
    }
    int visitGetGlobal([[maybe_unused]] wasm::GetGlobal *curr) {
        return 0;
    }
    int visitSetGlobal(wasm::SetGlobal *curr) {
        return visit(curr->value);
    }
    int visitLoad(wasm::Load *curr) {
        return visit(curr->ptr);
    }
    int visitStore(wasm::Store *curr) {
        return visit(curr->ptr) + visit(curr->value);
    }
    int visitAtomicRMW(wasm::AtomicRMW *curr) {
        return visit(curr->ptr) + visit(curr->value);
    }
    int visitAtomicCmpxchg(wasm::AtomicCmpxchg *curr) {
        return visit(curr->ptr) + visit(curr->expected) + visit(curr->replacement);
    }
    int visitAtomicWait(wasm::AtomicWait *curr) {
        return visit(curr->ptr) + visit(curr->expected) + visit(curr->timeout);
    }
    int visitAtomicWake(wasm::AtomicWake *curr) {
        return visit(curr->ptr) + visit(curr->wakeCount);
    }
    int visitSIMDExtract(wasm::SIMDExtract *curr) {
        return visit(curr->vec);
    }
    int visitSIMDReplace(wasm::SIMDReplace *curr) {
        return visit(curr->vec) + visit(curr->value);
    }
    int visitSIMDShuffle(wasm::SIMDShuffle *curr) {
        return visit(curr->left) + visit(curr->right);
    }
    int visitSIMDBitselect(wasm::SIMDBitselect *curr) {
        return visit(curr->cond) + visit(curr->left) + visit(curr->right);
    }
    int visitSIMDShift(wasm::SIMDShift *curr) {
        return visit(curr->vec) + visit(curr->shift);
    }
    int visitMemoryInit(wasm::MemoryInit *curr) {
        return visit(curr->dest) + visit(curr->offset) + visit(curr->size);
    }
    int visitDataDrop([[maybe_unused]] wasm::DataDrop *curr) {
        return 0;
    }
    int visitMemoryCopy(wasm::MemoryCopy *curr) {
        return visit(curr->dest) + visit(curr->source) + visit(curr->size);
    }
    int visitMemoryFill(wasm::MemoryFill *curr) {
        return visit(curr->dest) + visit(curr->value) + visit(curr->size);
    }
    int visitConst([[maybe_unused]] wasm::Const *curr) {
        return 0;
    }
    int visitUnary(wasm::Unary *curr) {
        return visit(curr->value);
    }
    int visitBinary(wasm::Binary *curr) {
        return visit(curr->left) + visit(curr->right);
    }
    int visitSelect(wasm::Select *curr) {
        return visit(curr->condition) + visit(curr->ifTrue) + visit(curr->ifFalse);
    }
    int visitDrop(wasm::Drop *curr) {
        return visit(curr->value);
    }
    int visitReturn(wasm::Return *curr) {
        if (curr->value) return visit(curr->value);
        return 0;
    }
    int visitHost(wasm::Host *curr) {
        int n = 0;

        for (const auto &expr : curr->operands) {
            n += visit(expr);
        }

        return n;
    }
    int visitNop([[maybe_unused]] wasm::Nop *curr) {
        return 0;
    }
    int visitUnreachable([[maybe_unused]] wasm::Unreachable *curr) {
        WASM_UNREACHABLE();
    }

    int visitFunctionType([[maybe_unused]] wasm::FunctionType *curr) {
        WASM_UNREACHABLE();
    }
    int visitExport([[maybe_unused]] wasm::Export *curr) {
        WASM_UNREACHABLE();
    }
    int visitGlobal([[maybe_unused]] wasm::Global *curr) {
        WASM_UNREACHABLE();
    }
    int visitFunction([[maybe_unused]] wasm::Function *curr) {
        WASM_UNREACHABLE();
    }
    int visitTable([[maybe_unused]] wasm::Table *curr) {
        WASM_UNREACHABLE();
    }
    int visitMemory([[maybe_unused]] wasm::Memory *curr) {
        WASM_UNREACHABLE();
    }
    int visitModule([[maybe_unused]] wasm::Module *curr) {
        WASM_UNREACHABLE();
    }
};

struct CounterPass : public wasm::Pass {
    std::ostream &o;

    explicit CounterPass(std::ostream &o)
        : o(o) {}

    void run([[maybe_unused]] wasm::PassRunner *runner, [[maybe_unused]] wasm::Module *module) override {
        CounterVisitor visitor{};
        int n = 0;

        for (const auto &f : module->functions) {
            n += visitor.visit(f->body);
        }

        o << n << std::endl;
    }

    bool modifiesBinaryenIR() override {
        return false;
    }
};

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << argv[0] << " <input file>" << std::endl;
        return 1;
    }

    wasm::Module module;
    wasm::ModuleReader{}.read(argv[1], module);

    wasm::PassRunner runner{&module};
    runner.add<CounterPass>(std::ref(std::cout));
    runner.run();
}
