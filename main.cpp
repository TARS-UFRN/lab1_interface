#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();



    int serial_fd;
    char *comando;

//    ufrn_al5d arm = new ufrn_al5d();
    ufrn_al5d arm;



    printf("PROGRAMA DEMONSTRACAO INICIADO\n\n");

    serial_fd = arm.abrir_porta(); // Instacia o objeto serial_fd


    if(serial_fd == -1) // -1 indica erro
    {
        printf("Erro abrindo a porta serial /dev/ttyS0\nAbortando o programa...");
        return -1;
    }
    else
    {
        printf("Porta serial /dev/ttyS0 aberta com sucesso\n");

        if(arm.configurar_porta(serial_fd) == -1)
        {
            printf("Erro inicializando a porta\n");
            close(serial_fd);
            return -1;
        }

        comando = (char*) malloc(sizeof(char)*BUFSIZE); //BUFSIZE Declarado como 1024 no header ufrn_al5d.h


        //////////////////////
        // PRIMEIRO COMANDO //
        //////////////////////
        printf("\nPRIMEIRO COMANDO - POSICAL INICIAL\n");

        sprintf(comando,"%s",HOME_POS); //Imprimi a posição inicial

        //Escrevendo com teste de escrita
        if(arm.enviar_comando(comando,serial_fd)!=-1)
        {
            printf("Envio de comando com teste de envio: %s\n",HOME_POS);
        }
        else
        {
            printf("Problema no envio do comando\nAbortando o programa...");
            return -1;
        }

        printf("Pressione enter para continuar...");
        getchar(); //Somente para esperar a próxima instrução

        /////////////////////
        // SEGUNDO COMANDO //
        /////////////////////
        printf("\nSEGUNDO COMANDO - MOVER O PUNHO\n");

        printf("Espere 5 segundos...\n");
        sleep(5);

        // NECESSÁRIO: Limpar a regiao de memória da string //
        //             antes de mandar outro comando        //
        // SINTAXE:
        //Sets the first BUFSIZE bytes of the block of memory pointed by ptr to the specified value (interpreted as an unsigned char).
        memset(comando, 0, BUFSIZE);

        // TESTE DA FUNCAO TRAVA //

        printf("Envio de comando sem teste de envio, utilizando a funcao trava: %s\n",comando);

        sprintf(comando,"#%dP%d",WRI_SERVO,arm.trava(WRI_SERVO,381231));
        arm.enviar_comando(comando,serial_fd);

        printf("Pressione enter para continuar...");
        getchar();

        //////////////////////
        // TERCEIRO COMANDO //
        //////////////////////
        printf("\nTERCEIRO COMANDO - MOVER A GARRA\n");

        printf("Espere 5 segundos...\n");
        sleep(5);

        memset(comando, 0, BUFSIZE);
        sprintf(comando,"#%dP%d",GRI_SERVO,arm.trava(GRI_SERVO,2000)); //trava é uma função do ufrn_al5d.h

        arm.enviar_comando(comando,serial_fd);

        printf("%s\n",comando);

        // FIM DO PROGRAMA DEMO //
        arm.fechar_porta(serial_fd);
        printf("\nAcesso a porta serial /dev/ttyS0 finalizado\n");

    }

    printf("\nPROGRAMA DEMONSTRACAO FINALIZADO\n\n");





    return a.exec();
}
