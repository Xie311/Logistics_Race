# Chssis__Code

Create time:2024-5

Editor:X311

## File tree

```
└─UserCode
    ├─Chassis
    │  ├─Callback
    │  │      Chassis_Callback.c
    │  │      Chassis_Callback.h
    │  │
    │  ├─Chassis_Debug
    │  │      chassis_debug.c
    │  │      chassis_debug.h
    │  │
    │  ├─Config
    │  │      Chassis_commen.h
    │  │      Chassis_UserConfig.h
    │  │      Chassis_Variable.h
    │  │
    │  ├─Perception
    │  │      chassis_perception.c
    │  │      chassis_perception.h
    │  │
    │  ├─Servo
    │  │      Chassis_Servo.c
    │  │      Chassis_Servo.h
    │  │
    │  ├─Start
    │  │      Chassis_Start.c
    │  │      Chassis_Start.h
    │  │
    │  └─StateMachine
    │          Chassis_StateMachine.c
    │          Chassis_StateMachine.h
    │
    └─Lib
        ├─Calculate
        │      wtr_calculate.c
        │      wtr_calculate.h
        │
        ├─Can
        │      wtr_can.c
        │      wtr_can.h
        │
        ├─Dji
        │      wtr_dji.c
        │      wtr_dji.h
        │
        ├─io_retargetToUart
        │      io_retargetToUart.c
        │      io_retargetToUart.h
        │
        ├─MAVlink
        │  │  checksum.h
        │  │  mavlink_conversions.h
        │  │  mavlink_get_info.h
        │  │  mavlink_helpers.h
        │  │  mavlink_sha256.h
        │  │  mavlink_types.h
        │  │  protocol.h
        │  │  wtr_mavlink.c
        │  │  wtr_mavlink.h
        │  │
        │  └─Chassis_State
        │          Chassis_State.h
        │          mavlink.h
        │          mavlink_msg_chassis.h
        │          testsuite.h
        │          version.h
        │
        ├─OPS
        │      wtr_ops.c
        │      wtr_ops.h
        │
        └─Target
                wtr_target.c
                wtr_target.h
```

## Known bug

- 在Debug线程内打印串口会寄，改到在StateMachine内打印Vofa数据正常

