#ifndef INC_STEPPER_MOTOR2_H_
#define INC_STEPPER_MOTOR2_H_

#include <stdint.h>

//外部声明
extern volatile uint32_t motor2_target_pulses;
extern volatile uint32_t motor2_current_pulses;
extern volatile uint8_t  motor2_is_running;

void Motor2_Speed_Dir(float v, uint8_t dir);
void Motor2_Speed_Dir_Angle(float v, uint8_t dir, float angle);
void Motor2_Stop(void);

#endif /* INC_STEPPER_MOTOR2_H_ */