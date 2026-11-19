#include<stdio.h>
int main(void){
    int cpu_state[8] = {0};
    //0 — счётчик команд
   //1 — аккумулятор
   //2 — регистр команд, хранит текущий опкод
   //3 — вектор прерываний (адрес обработчика ошибок)
   //4 — флаг остановки (0 = работает, 1 = остановлен)
   //5 — код последней ошибки (0 = нет ошибки)
   //6 — резерв для будущих расширений
   //7 — резерв для будущих расширений
    int memory[256] = {0};
    int i = 0;
    //считывание
    while(i < 256 && scanf("%d", &memory[i]) == 1){
        i++;
    }
    cpu_state[0] = 6;
    cpu_state[1] = 0;
    cpu_state[2] = 0;
    cpu_state[3] = memory[0];
    cpu_state[4] = 0;
    cpu_state[5] = 0;
    while(cpu_state[4] == 0 && cpu_state[5] == 0){
        int koman = cpu_state[0];
        if (cpu_state[0] < 0 || cpu_state[0] >= 256) { //проверка на вхождение cpu_state[0] в рамки массива памяти
            cpu_state[5] = 1;
            cpu_state[0] = cpu_state[3];
            continue;
        }

        cpu_state[2] = memory[cpu_state[0]];
        if(cpu_state[2] != 0){
            if(cpu_state[0] + 1 >= 256 || cpu_state[0]<0){
                cpu_state[5] = 1;
                cpu_state[0] = cpu_state[3];
                continue;
            }
            int adr_oper = cpu_state[0] + 1;
            int znach_oper = memory[adr_oper];
            if (znach_oper < 0 || znach_oper >= 256) {
                cpu_state[5] = 1; 
                cpu_state[0] = cpu_state[3]; 
                continue; 
            }

            switch(cpu_state[2]){
                case 1:
                    cpu_state[1] = memory[znach_oper];
                    break;
                case 2:
                    cpu_state[1] *= memory[znach_oper];
                    break;
                case 3:
                    if (memory[znach_oper] == 0) {
                        cpu_state[5] = 3; //деление на ноль
                        cpu_state[0] = cpu_state[3];
                        continue;
                    }
                    cpu_state[1] /= memory[znach_oper];
                    break;
                case 4:
                    cpu_state[0] = znach_oper;
                    break;
                case 5:
                    if (cpu_state[1] == 0){
                        cpu_state[0] = znach_oper;
                    }
                    else{
                        cpu_state[0] += 2;
                    }
                    break;
                default:
                    cpu_state[5] = 2;
                    cpu_state[0] = cpu_state[3];
                    continue;    
            }
            printf("pc=%d: команда %d, operand=%d, acc=%d\n", koman, cpu_state[2], znach_oper, cpu_state[1]);
        }
        else{
            printf("pc=%d: команда 0, acc=%d\n", koman, cpu_state[1]);
            cpu_state[4] = 1;
            continue;
        }
        //переход с шагом 2, пропуская операнды
        if (cpu_state[2] != 4 && cpu_state[2] != 5) {
            cpu_state[0] += 2;
        }
    }
    switch (cpu_state[5]){//ошибки
        case 0:
            printf("Программа завершена без ошибок.\n");
            break;
        case 1:
            printf("Ошибка. Выход за рамки памяти.\n");
            break;
        case 2:
            printf("Ошибка. Неизвестный опкод.\n");
            break;
        case 3:
            printf("Ошибка. Деление на ноль.\n");
            break;
    }
    return 0;
}