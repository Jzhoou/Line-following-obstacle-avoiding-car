# 循迹避障小车

## 项目说明

本仓库记录一个循迹避障小车团队项目。本人在团队中仅负责下位机部分，主要工作是 STM32F1 固件、传感器与执行器驱动以及整车控制逻辑；仓库内容不代表本人独立完成了机械结构、硬件制作或团队项目的全部工作。

## 下位机功能

- 以 STM32F1 微控制器为控制核心。
- 读取红外传感器状态，实现循迹判断与转向控制。
- 通过 Motor 驱动和 PWM 调节电机动作。
- 使用 USART 接收外部视觉/测距端发送的数据，并以 `Object_Distance` 和 `Barrier_Distance` 参与目标物与障碍判断。
- 使用蜂鸣器提示运行状态。

## 代码快照

`成品/1` 到 `成品/4` 是开发过程中的四个快照，用于保留不同阶段的实现。建议从 `成品/4` 开始阅读和使用；其根目录中的 `project.uvprojx` 是推荐打开的 Keil 工程文件。各快照内还保留了用于阶段性开发的子工程。

推荐快照的当前主路径在 `user/main.c` 中初始化 USART、红外循迹、电机、定时器和蜂鸣器。USART 接收中断解析外部视觉/测距数据并更新 `Object_Distance` 与 `Barrier_Distance`，主循环直接使用这两个变量执行目标物与障碍判断。

仓库同时保留了 HC-SR04、舵机和 OLED 驱动，供早期开发、独立实验或后续集成参考；其中舵机部分是保留的角度控制驱动。推荐快照的当前 `main.c` 没有调用 `HCSR04_Init()`、`Servo_Init()` 或 `OLED_Init()`，也没有执行舵机扫描，因此不能将这些模块描述为当前主路径中已经初始化并运行的功能。

仓库忽略 Keil 编译输出与本机用户配置，但保留 `.uvprojx`、源码、启动文件及其他打开和理解工程所需的配置。重新编译后，Keil 会在本地生成相应产物。

## 文件地图

- [`成品/4/user/main.c`](成品/4/user/main.c)：推荐快照的程序入口与循迹、避障决策主循环。
- [`成品/4/hardware/Serial.c`](成品/4/hardware/Serial.c)：USART 初始化、接收帧解析及两个距离变量的更新。
- [`成品/4/hardware/`](成品/4/hardware/)：电机、红外、蜂鸣器以及保留的 HC-SR04、舵机、OLED 等驱动。
- [`成品/4/project.uvprojx`](成品/4/project.uvprojx)：推荐打开的 Keil 工程。
- [`成品/`](成品/)：`1` 到 `4` 的开发快照集合。

## 使用提示

实际接线、引脚分配和控制流程请以对应快照中的源码与工程配置为准。使用前应完成硬件核对和现场调试；本仓库不声明固定的测距范围、行驶速度或循迹、避障成功率。

## English Summary

This repository documents a team-built line-following and obstacle-avoiding car. My contribution was limited to the lower-level STM32F1 firmware, peripheral drivers, and vehicle-control logic. Directories `成品/1` through `成品/4` are development snapshots, with `成品/4` as the recommended starting point.

In the recommended snapshot, the active `user/main.c` path initializes USART, infrared tracking, Motor/PWM control, a timer, and the buzzer. Its USART receive handler obtains external vision/ranging data and updates `Object_Distance` and `Barrier_Distance`; the main loop uses those values for object and barrier decisions. HC-SR04 and OLED drivers remain as experimental or retained modules, alongside a retained servo angle-control driver; the recommended `main.c` neither initializes the servo driver nor performs servo scanning, and it does not call `HCSR04_Init()` or `OLED_Init()`. The file map above links the active entry point, serial parser, drivers, and Keil project. No claims are made about distance, speed, or success-rate performance.
