# DLLEvents 说明

DebugEvent 虽然解决了漏判和拓展性的问题，但是毕竟是多个线程之间的协调问题，容易死锁，线程管理也较为困难。所以这次使用 DLL 注入的方式触发事件，pvzclass 作为注入器，启动后则关闭。

## 使用说明

请参照同一个 commit 中的 pvzclass.cpp 文件。

首先，注入 DLL，调用初始化函数。immediateExecute 是用于立即执行下两行的注入代码的，否则只有在游戏 Update 的时候才会执行。

```cpp
PVZ::Memory::immediateExecute = true;
PVZ::Memory::InjectDll("pvzdll.dll");
PVZ::Memory::InvokeDllProc("init");
```

然后，使用 `CoinCollectEvent e = CoinCollectEvent();` 的方式注册事件，使用 `e.end();` 的方式结束事件。

监听函数的实现请参照 pvzdll 子项目中的 pch 文件。每一个 DLLEvent 都有一个对应的函数，例如 onCoinCollect 对应 CoinCollectEvent，在其中编写响应即可。

最后，生成项目，Release 文件夹中会有 pvzclass.exe 和 pvzdll.dll 这两个文件，全部复制到游戏根目录启动即可。

## 开发说明

请参照 Events 文件夹中相关文件。

首先，继承 DLLEvent 的基类，然后在构造函数中编写以下代码，以 CoinCollectEvent 为例解释：

```cpp
CoinCollectEvent::CoinCollectEvent()
{
    int procAddress = PVZ::Memory::GetProcAddress("onCoinCollect");
    hookAddress = 0x432060;
    rawlen = 6; // 应当>=5
    BYTE code[] = { 0x51, INVOKE(procAddress), ADD_ESP(4) };
    start(STRING(code));
}
```

第一行 `"onCoinCollect"` 对应 pch 中的相应函数。

第二行 `hookAddress` 为欲注入的入口的地址。

第三行 `rawlen` 代表需要将多少长度的地址替换为跳转语句，至少为 5 字节，例如：

```
PlantsVsZombies.exe+32060 - 55                    - push ebp
PlantsVsZombies.exe+32061 - 8B EC                 - mov ebp,esp
PlantsVsZombies.exe+32063 - 83 E4 F8              - and esp,-08
```

这段中，跳转语句需要 5 字节，所以需要将这 6 个字节全部替换为跳转语句，也就是语句需要完整替换。你只需要将 rawlen 设定好，跳转语句和 NOP 会在 `start()` 中自动完成。 **注意！替换的原始代码中不能存在偏移量，例如 JMP 或 CALL！**

第四行 `code[]` 是注入的代码，需要调用 `procAddress` 的对应函数，默认的 `__cdecl` 调用约定需要将参数压入栈，然后由调用方清理堆栈。 **注意！如果需要对栈进行操作，由于框架会自动 PUSHAD 和 POPAD，所以偏移值需要加 32！** 例如，`push [esp+4]` 需要使用 `PUSH_PTR_ESP_ADD_V(36)` 来实现。此外，如果需要在注入的代码结束后将返回值赋给寄存器，则需要将 EAX 赋值给栈中 PUSHAD 对应的寄存器偏移量。

第五行 `start(STRING(code))` 原样照抄即可。
