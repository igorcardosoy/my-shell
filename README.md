# Projeto Shell

Este repositório contém todo o código do projeto Shell, que consiste na criação de um terminal baseado na interface Posix. Utilizamos estruturas de dados para implementar alguns comandos internos e o execvp para comandos externos. Todo o código foi escrito em C.

## Autores

- [Cauã Rufino de Sá](https://github.com/CauaDeSa)
- [Igor Filipi Cardoso](https://github.com/IgorFilipiCardoso)

## Compilar e Executar

 - Para compilar o programa, execute o comando:
 ```bash
  $ make 
 ```

- Se preferir compilar manualmente, execute o comando:
```bash
  $ gcc -o my-shell my-shell.c ./utils/utils.c ./queue/queue_impl.c ./lse/lse_impl.c
```

Para executar o programa, execute o comando:
```bash
  $ ./my-shell
```

## Instalar Make
```bash
  $ sudo apt install make
```


