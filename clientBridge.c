#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include "clientBridge.h"

void write_struct(int fd, unsigned long command, struct complex_struct * struct_address){
    if (ioctl(fd, command, struct_address) == -1){
        perror("Write message error at ioctl");
    }
}

void write_message(int fd, unsigned long command, char * message){
    if (ioctl(fd, command, message) == -1){
        perror("Write message error at ioctl");
    }
}

void read_message(int fd, unsigned long command, char * message){
    if(ioctl(fd, command, message) == -1){
	perror("Read message error at ioctl");
    }
}

void read_message_param(int fd, unsigned long command, int * value){
    if(ioctl(fd, command, value) == -1){
        perror("Read message param error at ioctl");
    }else{
        printf("Copy the messsage from the kernel\n");
    }
}

void write_int(int fd, unsigned long command, int * value){
    if (ioctl(fd, command, value) == -1){
        perror("Write int error at ioctl");
    }
}

void read_int(int fd, unsigned long command, int * value){
    if(ioctl(fd, command, value) == -1){
        perror("Read int error at ioctl");
    }else{
        printf("Copy the int from the kernel\n");
    }
}

int send_empty_command(int fd, unsigned long command){
    int returned_value = ioctl(fd, command);
    if(returned_value == -1){
       perror("Send command error at ioctl");
    }else{
	return returned_value;
        printf("Command OK to the kernel\n");
    }
}


// void write_several_messages(int fd){
//     write_message(fd, BRIDGE_W_S, "Message 1");
//     write_message(fd, BRIDGE_W_S, "Message 2");
//     write_message(fd, BRIDGE_W_S, "Message 3");
// }

void read_all_messages_s(int fd){
	char message[100];
	while(send_empty_command(fd, BRIDGE_STATE_S) > 0){
	    read_message(fd, BRIDGE_R_S, message);
	    printf("%s", message);
	}
}


void create_s(int fd){
    send_empty_command(fd, BRIDGE_CREATE_S);
    printf("\nse creó el stack.\n");
}

struct list_struct
{
	char* message;
	struct list_head list;
	
};

/**
 * @brief Crea un nodo tipo list_struct, le agrega el dato al nodo.
 * ESte nodo que devuelve el método se debe agregar a la listacon el 
 * siguiente comando : list_add(&(node->list),&list_head);
 * 
 * @param message 
 * @return struct list_struct* : nodo completo para agregar a la lista  con 
 */
struct list_struct* create_l(char* message){
    struct list_struct *node;
    //char *date = message;
    
    node = (struct list_struct*)malloc(sizeof(struct list_struct));

    node -> message = message;
    INIT_LIST_HEAD( &node -> list );

    return node;
}

/**
 * @brief Error en la clase, imprime en circulo...
 * 
 * @param node_head 
 */
void read_all_messages_l(struct list_head node_head){
    struct list_struct *pos;
    list_for_each_entry(pos,&node_head,list){
        //printf("%p\t%s -> ",&pos->list, pos->message);
        printf("%s -> ", pos->message);
    }
}

void main_list(int fd){

    //Crear los nodos con el dato
    struct list_struct *pos;
    char* message1 = "01";
    struct list_struct *node1 = create_l(message1);
    char* message2 = "02";
    struct list_struct *node2 = create_l(message2);
    char* message3 = "03";
    struct list_struct *node3 = create_l(message3);
    char* message4 = "04";
    struct list_struct *node4 = create_l(message4);
     
    //nodo cabeza
    LIST_HEAD(list_head);

    //Añador los nodos en el nodo cabeza
    list_add(&(node1->list),&list_head);
    list_add(&(node2->list),&list_head);
    list_add(&(node3->list),&list_head);
    list_add(&(node4->list),&list_head);


    //read_all_messages_l(list_head, pos);
    // Mostrar datos de los nodos
    list_for_each_entry(pos,&list_head,list){
        //printf("%p\t%s -> ",&pos->list, pos->message);
        printf("%s -> ", pos->message);
    }

    
    // Eliminar la lista y liberar los nodos 
    list_del(&(node1->list));
    free(node1);
    list_del(&(node2->list));
    free(node2);
    list_del(&(node3->list));
    free(node3);
    list_del(&(node4->list));
    free(node4);
    
}


void firstPoint(int fd) {
    char filePath[100];
    printf("\n1. Reverse the lines of a text file.");
    printf("\n   Input the file path: ");
    scanf("%s", filePath);

    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    fp = fopen(filePath, "r");
    if (fp == NULL) {
        perror("First point open file.");
    }

    printf("\n File content:\n");
    create_s(fd);
    while ((read = getline(&line, &len, fp)) != -1) {
        printf("%s", line);
        write_message(fd, BRIDGE_W_S, line);
    }
    printf("\n Reversed file:\n");
    read_all_messages_s(fd);

    fclose(fp);
    if (line)
        free(line);
}

void secondPoint(int fd){
    char filePath[100];
    printf("2. Barajar lineas de archivo.\n");
    printf("   Nombre del archivo: \n");
    scanf("%s", filePath);

    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    fp = fopen(filePath, "r+");
    if (fp == NULL) {
        perror("First point open file.");
    }

    printf("\n  Archivo inicial:\n");
    while ((read = getline(&line, &len, fp)) != -1) {
        printf("%s", line);
        write_message(fd, BRIDGE_W_L, line);
    }

	char message[100];
    while( send_empty_command(fd, BRIDGE_STATE_L)>0){
        read_message(fd, BRIDGE_R_L, message);
        fputs(message, fp);
        printf("%s", message);
    }

    printf("\n Archivo barajado:\n");
    //read_all_messages_l(fd);

    fclose(fp);
    if (line)
        free(line);

}

void menu(int fd) {
    int opcion = 99; // Just don't init with 0 plz
    do{
        printf("\n === First Practice Menu === \n");
        printf("0. Exit.\n");
        printf("1. roy.\n");
        printf("2. vane.\n");
        printf("3. roy.\n");
        printf("4. Gab.\n");
        printf("5. .\n");
        printf("6. .\n");
        printf("7. .\n");
        printf("8. .\n");
        printf("9. .\n");
        printf("10. .\n");
        printf("Input which point of this practice you wanna try: ");
        scanf("%d", &opcion);
        switch (opcion)
        {
            case 1:
                firstPoint(fd);
                break;
            case 2:
                main_list(fd);
                //secondPoint(fd);
                break;
            case 3:
            case 4:
            case 5:
            case 6:

        }
        printf("\nThe selected option was: ");
        printf("%d\n", opcion);
    }
    while(opcion != 0);
}

int main(int argc, char *argv[]){

    const char *file_name = "/dev/bridge"; //used by ioctl
    int fd;

    fd = open(file_name, O_RDWR);
    if (fd == -1){
        perror("Bridge ioctl file open");
        return 2;
    }

    char message[] = "mensajeEspecifico";
    int value = 10;

    //send_empty_command(fd, BRIDGE_CREATE_Q);                    //Create a queue
    //write_message(fd, BRIDGE_W_HIGH_PRIOR_Q, message);		//Send message with high priority
    //write_message(fd, BRIDGE_W_MIDDLE_PRIOR_Q, message);	//Send message with middle priority
    //write_message(fd, BRIDGE_W_LOW_PRIOR_Q, message);		//Send message with low priority
    //read_message(fd, BRIDGE_R_HIGH_PRIOR_Q, message);		//Read a message with high priority
    //read_message(fd, BRIDGE_R_MIDDLE_PRIOR_Q, message);		//Read a message with middle priority
    //read_message(fd, BRIDGE_R_LOW_PRIOR_Q, message);		//Read a message with low priority
    //send_empty_command(fd, BRIDGE_STATE_Q);			//Get an int indicating the state of the queue
    //send_empty_command(fd, BRIDGE_DESTROY_Q);			//Destroy a queue completely releasing memory (IMPORTANT!!)

    //send_empty_command(fd, BRIDGE_CREATE_S);                   	//Create a stack
    //write_message(fd, BRIDGE_W_S, "Message WS");		//Write a message in the stack
    //read_message(fd, BRIDGE_R_S, message);			//Read a message from the stack
    //send_empty_command(fd, BRIDGE_STATE_S);			//Get an int indicating the statte of the stack
    //send_empty_command(fd, BRIDGE_DESTROY_S);			//Destroy a stack completely releasing the memory (IMPORTANT!!)

    //send_empty_command(fd, BRIDGE_CREATE_L);                    //Create a list
    //write_message(fd, BRIDGE_W_L, message);			//Write a message in the last position of a list
    //read_message(fd, BRIDGE_R_L, message);			//Read a message of the last position of a list
    //send_empty_command(fd, BRIDGE_INVERT_L);			//Invert the order of the elements of a list
    //write_int(fd, BRIDGE_ROTATE_L, &value);			//Rotate a list value position to the right
    //send_empty_command(fd, BRIDGE_CLEAN_L);			//Eliminate repeated elements in a list
    //read_message(fd, BRIDGE_GREATER_VAL_L, message);		//Find the greater value of a list of strings
    //printf("Value obtained: %s\n", message);
    //send_empty_command(fd, BRIDGE_END_L);			//End a list (to be able to create another list) (Module limited to at most three list)
    //send_empty_command(fd, BRIDGE_CONCAT_L);			//Concatenate two previous list in a third new one
    //send_empty_command(fd, BRIDGE_STATE_L);			//Get an int indicating the state of a list
    //send_empty_command(fd, BRIDGE_DESTROY_L);			//Destroy all the list of the module releasing memory (IMPORTANT!!)

    // struct complex_struct tmp;
    // strcpy((tmp.messages)[0],"Complex struct message 1");
    // strcpy((tmp.messages)[1], "Complex struct message 2");
    // strcpy((tmp.messages)[2], "Complex struct message 3");
    // tmp.value = 3;
    // write_struct(fd, BRIDGE_W_CS, &tmp);
    // write_several_messages(fd);
    // read_all_messages_s(fd);
    menu(fd);
    close (fd);
    return 0;
}