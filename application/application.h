#if defined(STM32L432xx)
#define BUTTON_PORT   B1_GPIO_Port
#define BUTTON_PIN    B1_Pin
#define TRIGGER_PORT  B1_GPIO_Port
#define TRIGGER_PIN   B1_Pin
#else
#define BUTTON_PORT   B1_GPIO_Port
#define BUTTON_PIN    B1_Pin
#define TRIGGER_PORT  B1_GPIO_Port
#define TRIGGER_PIN   B1_Pin
#endif






int applicationInit(void);
int applicationLoop(void);
