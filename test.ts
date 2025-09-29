// test.ts - 简化的测试代码

// 初始化串口
serial.redirect(SerialPin.P1, SerialPin.P0, BaudRate.BaudRate115200)

// 简单的测试函数
input.onButtonPressed(Button.A, function () {
    serial.writeLine("AT")
    basic.showString("AT Sent")
})

input.onButtonPressed(Button.B, function () {
    let result = emakefun.multiFindUtil(["OK", "ERROR"], 2000)
    if (result == 0) {
        basic.showIcon(IconNames.Yes)
    } else if (result == 1) {
        basic.showIcon(IconNames.No)
    } else {
        basic.showIcon(IconNames.Sad)
    }
})

// 初始化显示
basic.showString("ESP-AT Test")