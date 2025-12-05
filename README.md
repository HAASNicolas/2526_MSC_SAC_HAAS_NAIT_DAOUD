# 2526_MSC_SAC_HAAS_NAIT_DAOUD
## Participants
* Elias NAIT DAOUD
* Nicolas HAAS

## Génération des 4 PWM
On veut une fréquence de 20kHz pour les PWM, avec une résolution minimum de 10bits.

La carte STM32 a une fréquence de 170MHz

170MHz / 20kHz = 8500
Il faut diviser la fréquence de la carte STM32 par 8500 pour obtenir une fréquence de 20kHz pour la PWM.
Il faut que (PSC + 1)*(ARR + 1) = 8500, donc PSC = 4 et ARR = 2125

Le temps mort minimum est de 39ns :

<img width="719" height="73" alt="temps_mort_min" src="https://github.com/user-attachments/assets/49761359-79b0-4f6c-9dce-8f08736bc129" />

On veut un rapport cyclique de 60%.
alpha = CCR/ARR <=> CCR = ARR * alpha => 8500 * 0.6 = 5100.
On a donc fixé CNT à 5100

<img width="800" height="480" alt="tek00A000" src="https://github.com/user-attachments/assets/f5fc29eb-de17-4460-9ab9-802295889b5c" />

La vision à l'oscilloscope nous permet de vérifier que la fréquence de TIM1_CHANNEL_1 et TIM1_CHANNEL_2 est bien de 20kHz.
