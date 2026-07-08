# K60 智能车蓝牙遥控 - 开发计划

## 目标
通过 TTL 或经典蓝牙串口（SPP，lq_btm2v2 / BK5B 模块）实现对 K60 智能车的遥控：
- 舵机转向
- 电机低速前进/后退
- 串口命令简单可控

## 当前状态
- [x] UART4 通信已通（TX/RX 对调解决）
- [x] TTL 模块连接成功，可正常收发数据
- [x] 舵机和电机功能已完全可用
- [x] 遥控命令系统已实现（L/R/C/F/B/S）
- [x] 编码器功能已初始化

## 硬件接口与引脚
- 舵机：PTB19，使用 `FTM2 CH1` 输出
- 电机：PTC1 / PTC2 / PTC3 / PTC4，使用 `FTM0 CH0..CH3` 输出
- UART4：蓝牙/TTL 串口

## 修改范围
仅修改以下文件：
- `src/User/main.c`
- `src/Source/UART.c`

不修改 FTM 库函数，不改原始电机/舵机驱动逻辑实现。

## main.c 细化改动
1. 去掉 `Test_ALL()` 调用
2. 添加与原始例程一致的 PWM 初始化：
   - 舵机 PWM：`Servo_FTM_PWM_Init(FTM2, FTM_CH1, bus_clk * 1000000 / 64 / 50, Step_Middle);`
   - 电机四通道 PWM：
     - `FTM_PWM_Init(FTM0, FTM_CH0, bus_clk * 1000000 / 12500 / 8, 0);`
     - `FTM_PWM_Init(FTM0, FTM_CH1, bus_clk * 1000000 / 12500 / 8, 0);`
     - `FTM_PWM_Init(FTM0, FTM_CH2, bus_clk * 1000000 / 12500 / 8, 0);`
     - `FTM_PWM_Init(FTM0, FTM_CH3, bus_clk * 1000000 / 12500 / 8, 0);`
3. 启用 UART4 中断：`UART_Irq_En(UART4);`

## UART.c 细化改动
### 命令格式
- `L`：舵机左转一个固定增量，执行 `FTM_PWM_Duty(FTM2, FTM_CH1, current_servo_pwm - step_servo)`
- `R`：舵机右转一个固定增量，执行 `FTM_PWM_Duty(FTM2, FTM_CH1, current_servo_pwm + step_servo)`
- `C`：舵机回中，执行 `FTM_PWM_Duty(FTM2, FTM_CH1, Step_Middle)`
- `F`：电机增加前进速度一个固定增量，`target_speed += 5`
- `B`：电机增加后退速度一个固定减量，`target_speed -= 5`
- `S`：电机停止，执行 `CH0/CH1/CH2/CH3 = 1000`
- `Q`：查询当前状态或回显参数

### 解析规则
- 只支持单字符命令，避免复杂数值输入
- `L` / `R` 直接按固定转向步长改变当前舵机 PWM，建议步长 `40`
- `F` 每次按下时增加 `target_speed` 5 个单位
- `B` 每次按下时减少 `target_speed` 5 个单位
- `S` 立即停止，所有电机通道设回 `1000`
- `Q` 查询当前状态或回显参数
- 支持换行结束：`\r`, `\n`, 或 `\r\n`
- 非法命令应忽略并返回错误提示（如果当前串口回显可用）

### 低速行驶约束
- 速度增量固定为 `5`
- 目标速度范围按实际硬件调整，不要在程序里强行限制成 `-40 ~ 40`
- 每次 `F` / `B` 都是小步进，避免一次性加速过猛
- `L` / `R` 方向增量建议为 `40`
- `S` 应立即把所有电机通道设回 `1000`

### 具体电机PWM映射
- `F`：
  - `FTM_PWM_Duty(FTM0, FTM_CH0, 1000);`
  - `FTM_PWM_Duty(FTM0, FTM_CH2, 1000);`
  - `FTM_PWM_Duty(FTM0, FTM_CH1, 1000 - current_speed_pwm);`
  - `FTM_PWM_Duty(FTM0, FTM_CH3, 1000 - current_speed_pwm);`
- `B`：
  - `FTM_PWM_Duty(FTM0, FTM_CH0, 1000 - current_speed_pwm);`
  - `FTM_PWM_Duty(FTM0, FTM_CH2, 1000 - current_speed_pwm);`
  - `FTM_PWM_Duty(FTM0, FTM_CH1, 1000);`
  - `FTM_PWM_Duty(FTM0, FTM_CH3, 1000);`
- `S`：
  - `FTM_PWM_Duty(FTM0, FTM_CH0, 1000);`
  - `FTM_PWM_Duty(FTM0, FTM_CH1, 1000);`
  - `FTM_PWM_Duty(FTM0, FTM_CH2, 1000);`
  - `FTM_PWM_Duty(FTM0, FTM_CH3, 1000);`

## 低速调试建议
1. 串口先测试 `C`：舵机回中
2. 再测试 `L`, `R`：确认舵机方向是否正确，并检验转向增量
3. 再测试 `F`, `B`：确认电机低速加速/减速是否生效
4. 若车身移动不稳定，可逐步增加速度步长 `step_speed`，但不要过大
5. 确认电机停止命令 `S` 能立即回到中值

## PID 低速闭环方案
### 编码器/解码器使用
- [x] 使用 `FTM1` 的正交解码功能：`FTM_AB_Init(FTM1);`
- [x] 读取编码器计数：`FTM_AB_Get(FTM1);`
- [x] 建议接入的传感器引脚为 `PTA12 / PTA13`，对应 `FTM1_QDPHA` 和 `FTM1_QDPHB`
- [x] `FTM_AB_Get` 每次读取后会清零计数，可用于定时速度采样

### PID 控制逻辑
- [ ] 实现 PID 控制器（Kp、Ki、Kd 参数）
- [ ] 目标：把开启命令 `F<val>` / `B<val>` 作为目标速度输入
- [ ] 反馈：从编码器采样得到当前速度 `speed_feedback`
- [ ] 控制量：计算 PWM 调整量 `motor_pwm = pid_output + base_pwm`
- [ ] 输出：通过 `FTM_PWM_Duty(FTM0, ...)` 控制电机前进/后退

### 推荐实现步骤
1. 在 `main.c` 中初始化编码器：
   - `FTM_AB_Init(FTM1);`
   - 如果需要，可增加周期定时器 `PIT` 或 `LPTMR`，比如 50ms/100ms 采样一次
2. 在定时中断或主循环中执行：
   - 读取编码器计数：`int16 pulse = FTM_AB_Get(FTM1);`
   - 计算 `current_speed`：根据采样周期和脉冲数换算速度值
   - 计算 PID 输出：`error = target_speed - current_speed`
   - 生成电机 PWM：`motor_pwm = constrain(pid_output, 0, max_pwm)`
3. 让 `F/B` 命令设置 `target_speed`，而不是直接输出固定 PWM
4. `S` 命令清零目标速度，并立即把所有电机通道设回 `1000`

### PID 参数建议
- 初始参数：`Kp = 0.5 ~ 2.0`，`Ki = 0.05 ~ 0.2`，`Kd = 0 ~ 0.1`
- 先仅使用比例控制 `Kp` 观察响应，再逐步加积分 `Ki`
- 目标低速闭环范围：`target_speed = 10 ~ 40`（单位为编码器计数/周期，或按你实际算法换算）
- 低速行驶时，避免设置过大的 `base_pwm`

### 兼容旧命令方式
- 如果目前只要先实现测试，可以先把 `F<val>` / `B<val>` 映射为目标速度
- 后续可扩展命令：`V<val>` 设置目标速度，`T` 启用 PID，`G` 查询速度

## 参考位置
- 参考 `LQKEY.c` 中舵机 `FTM_PWM_Duty` 调用位置
- 参考 `LQKEY.c` 第 251-269 行原始电机 Duty 计算方式
- 参考 `FTM.c` 中 `FTM_AB_Init`、`FTM_AB_Get` 的编码器读数实现

## LED指示灯

- 要求程序运行过程中有一个LED等1s闪烁一次

## 成果验收条件
- `UART4` 能接收蓝牙/TTL 命令
- 舵机能响应 `L/R/C` 命令
- 电机能响应 `F/B/S` 命令
- 使用 `F20` / `B20` 能实现缓慢前进/后退
- `S` 命令能可靠停止运动

---

## 🚀 优化任务列表（待完成）

### 1. PID 闭环控制 - ⭐ 高优先级 ✅ 已完成（已修复）
- [x] 实现 PID 算法框架，添加 Kp、Ki、Kd 参数结构体
- [x] 集成编码器反馈获取实际速度
- [x] 在主循环中定时更新 PID 计算（建议 10-20ms 周期）
- [x] 校准 PID 参数使小车速度稳定
- [x] 不同地面/电池电压下速度保持一致

### 2. 舵机平滑过渡 - 中优先级
- [ ] 添加舵机目标值平滑缓冲
- [ ] 实现舵机渐进式转向（避免过急转向）
- [ ] 减少地面冲击和机械损耗

### 3. 利用 MPU6050 传感器 - 中优先级
- [ ] 初始化并读取陀螺仪数据（gyro_x/y/z）
- [ ] 读取加速度计数据（accle_x/y/z）
- [ ] 实现姿态稳定控制（补偿倾斜）
- [ ] 冲撞检测（加速度突增告警）
- [ ] 转向辅助（陀螺仪反馈）

### 4. 优化主循环时序 - 中优先级
- [ ] 改为中断驱动的控制框架
- [ ] 编码器使用中断更新速度
- [ ] PIT/PLL 定时器执行控制算法（固定周期）
- [ ] 主循环仅处理通信和高层逻辑

### 5. 实时数据监控与诊断 - 中优先级
- [ ] 添加串口实时上报功能：当前 PWM、实际速度、目标速度等
- [ ] 实现查询命令（如 `Q` 显示当前状态）
- [ ] 上位机可视化监控工具

### 6. 能耗优化 - 低优先级
- [ ] 添加 ADC 采样电池电压
- [ ] 实现 PWM 补偿算法（电压低时自动提高 PWM）
- [ ] 运行时间统计与续航评估
- [ ] 低电压告警
