# 循迹避障小车

## 项目说明

本仓库记录一个循迹避障小车团队项目。本人在团队中仅负责下位机部分，主要工作是 STM32F1 固件、传感器与执行器驱动以及整车控制逻辑；仓库内容不代表本人独立完成了机械结构、硬件制作或团队项目的全部工作。

## 下位机功能

- 以 STM32F1 微控制器为控制核心。
- 读取红外传感器状态，实现循迹判断与转向控制。
- 使用 HC-SR04 超声波模块感知前方障碍。
- 通过 Motor 驱动和 PWM 调节电机动作。
- 驱动舵机扫描不同方向，为避障决策提供环境信息。
- 使用蜂鸣器提示运行状态，并通过 OLED 显示调试或状态信息。

## 代码快照

`成品/1` 到 `成品/4` 是开发过程中的四个快照，用于保留不同阶段的实现。建议从 `成品/4` 开始阅读和使用；其根目录中的 `project.uvprojx` 是推荐打开的 Keil 工程文件。各快照内还保留了用于阶段性开发的子工程。

仓库忽略 Keil 编译输出与本机用户配置，但保留 `.uvprojx`、源码、启动文件及其他打开和理解工程所需的配置。重新编译后，Keil 会在本地生成相应产物。

## 使用提示

实际接线、引脚分配和控制流程请以对应快照中的源码与工程配置为准。使用前应完成硬件核对和现场调试；本仓库不声明固定的测距范围、行驶速度或循迹、避障成功率。

## English Summary

This repository documents a team-built line-following and obstacle-avoiding car. My contribution was limited to the lower-level STM32F1 firmware, peripheral drivers, and vehicle-control logic. The project uses infrared line sensors, an HC-SR04 ultrasonic module, Motor/PWM control, servo scanning, a buzzer, and an OLED. Directories `成品/1` through `成品/4` are development snapshots; `成品/4` is the recommended starting point. No claims are made about distance, speed, or success-rate performance.
