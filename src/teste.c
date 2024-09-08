// #include <stdio.h>
// #include <string.h>

// int main() {

//     char line[2000];
//     scanf("%[^\n]", line);
//     // char input[] = "ps aux # ls -a -a # mkdir pasta # htop";
    
//     // Primeira separação pelos comandos, usando '#'
//     char* command = strtok(line, "#");
    
//     char* commands[5];
//     int count = 0;
//     while (command != NULL) {
//         printf("Comando: %s\n", command);
//         commands[count] = strdup(command);
        
//         // Avançar para o próximo comando separado por '#'
//         command = strtok(NULL, "#");
//         count++;
//     }

//     for (int i = 0; i < count; i++) {
//         // Agora separa os parâmetros com '-'
//         char* dup = strdup(commands[i]);
//         char* parameter = strtok(dup, "-");
        
//         // O primeiro token pode ser o nome do comando, imprimimos ele primeiro
//         if (parameter != NULL) {
//             printf("Comando principal: %s\n", parameter);

//         }
        
//         // Pegamos os parâmetros com '-'
//         parameter = strtok(NULL, "-");
//         while (parameter != NULL) {
//             printf("Parâmetro: -%s\n", parameter);  // Adiciona o '-' de volta para a saída
//             parameter = strtok(NULL, "-");
//         }
//     }

//     return 0;
// }
