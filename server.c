#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include <stdbool.h>
#include <ctype.h>

#define PORT 2908

typedef struct thData
{
  int idThread;
  int cl;
} thData;

extern int errno;
static void *treat(void *);
void raspunde(void *);

int isNumber(const char *str)
{
  for (int i = 0; str[i] != '\0'; i++)
  {
    if (!isdigit(str[i]))
    {
      return 0;
    }
  }
  return 1;
}

int main()
{
  struct sockaddr_in server;
  struct sockaddr_in from;
  int nr;
  int sd;
  int pid;
  pthread_t th[100];
  int i = 0;

  if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
  {
    perror("[server]Eroare la socket().\n");
    return errno;
  }

  int on = 1;
  setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

  bzero(&server, sizeof(server));
  bzero(&from, sizeof(from));

  server.sin_family = AF_INET;
  server.sin_addr.s_addr = htonl(INADDR_ANY);
  server.sin_port = htons(PORT);

  if (bind(sd, (struct sockaddr *)&server, sizeof(struct sockaddr)) == -1)
  {
    perror("[server]Eroare la bind().\n");
    return errno;
  }
  if (listen(sd, 2) == -1)
  {
    perror("[server]Eroare la listen().\n");
    return errno;
  }
  while (1)
  {
    int client;
    thData *td;
    int length = sizeof(from);

    printf("[server]Asteptam la portul %d...\n", PORT);
    fflush(stdout);

    if ((client = accept(sd, (struct sockaddr *)&from, &length)) < 0)
    {
      perror("[server]Eroare la accept().\n");
      continue;
    }

    td = (struct thData *)malloc(sizeof(struct thData));
    td->idThread = i++;
    td->cl = client;

    pthread_create(&th[i], NULL, &treat, td);
  }
};
static void *treat(void *arg)
{
  struct thData tdL;
  tdL = *((struct thData *)arg);
  printf("[thread] - %d - Asteptam mesajul...\n", tdL.idThread);
  fflush(stdout);
  pthread_detach(pthread_self());
  raspunde((struct thData *)arg);
  return NULL;
}

void raspunde(void *arg)
{
  int nr, i = 0;
  struct thData tdL;
  tdL = *((struct thData *)arg);
  char text[4096];
  char ip[16];

  while (1)
  {
    char adresa_ip[22];
    char ip_adress[22];
    bool security= false;
    bool ip_primit = false; // Variabila prin care stie serverul ca trebuie sa verifice existenta unui agent prin IP
    bool ip_gasit = false;
    bool log_cerut = false;                                                                                      // Variabila prin care stie serverul sa ia log-uri si sa le trimita la utilizator
    bool all_logs = false, security_logs = false, kernel_logs = false, priority_logs = false, boot_logs = false; // tipurile de log-uri ceruite de utilizator
    bool logs_10 = false, logs_20 = false, logs_50 = false;
    bool recent_logs = false, today_logs = false, week_logs = false, month_logs = false;
    bzero(text, sizeof(text));
    if (read(tdL.cl, text, sizeof(text)) <= 0)
    {
      printf("[Thread %d]\n", tdL.idThread);
      perror("Eroare la read() de la client.\n");
      break;
    }
     if (strstr(text, "banana") != NULL) // aici trimitem la user incidentele de securitate
     {
      security = true;
      printf("Suntem in security management!\n");
      if (sscanf(text, "%*[^0-9.]%15[0-9.]", ip_adress) == 1)
      {
        printf("Adresa IP in security: %s\n", ip_adress);
      }
      else
      {
        printf("Adresa IP nu a putut fi găsita in sir.\n");
      }

    }
    else if (strstr(text, "add") != NULL)
    {
      ip_primit = true;
      system("ls /var/log/journal/remote > ip_list.txt");
      FILE *file = fopen("ip_list.txt", "r");
      if (file == NULL)
      {
        perror("Eroare la deschiderea fisierului ip_list.txt");
      }
      if (sscanf(text, "add_ip %15s", ip) == 1)
      {
        printf("Adresa IP extrasa: %s\n", ip);
      }
      else
      {
        printf("Nu s-a gasit adresa IP in text.\n");
      }
      char line[4096];
      while (fgets(line, sizeof(line), file) != NULL)
      {
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n')
        {
          line[len - 1] = '\0';
        }

        if (strstr(line, ip) != NULL)
        {
          ip_gasit = true;
          printf("Adresa IP %s exista in fisierul ip_list.txt\n", ip);
          fclose(file);
          break;
        }
      }
    }
    else if (strstr(text, "reque") != NULL) // Comanda de cerere a log-urilor
    {

      if (sscanf(text, "%*[^0-9.]%15[0-9.]", adresa_ip) == 1)
      {
        printf("Adresa IP: %s\n", adresa_ip);
      }
      else
      {
        printf("Adresa IP nu a putut fi găsita in sir.\n");
      }

      log_cerut = true;
      if (strstr(text, "All") != NULL)
      {
        all_logs = true;
        printf("ALL\n");
      }
      if (strstr(text, "Security") != NULL)
      {
        security_logs = true;
        printf("Security\n");
      }
      if (strstr(text, "Kernel") != NULL)
      {
        kernel_logs = true;
        printf("Kernel\n");
      }
      if (strstr(text, "Priority") != NULL)
      {
        priority_logs = true;
        printf("Priority\n");
      }
      if (strstr(text, "Boot") != NULL)
      {
        boot_logs = true;
        printf("Boot\n");
      }
      if (strstr(text, "10") != NULL)
      {
        logs_10 = true;
        printf("10\n");
      }
      if (strstr(text, "20") != NULL)
      {
        logs_20 = true;
        printf("20\n");
      }
      if (strstr(text, "50") != NULL)
      {
        logs_50 = true;
        printf("50\n");
      }
      if (strstr(text, "Rece") != NULL)
      {
        recent_logs = true;
        printf("Recent\n");
      }
      if (strstr(text, "Tod") != NULL)
      {
        today_logs = true;
        printf("Today\n");
      }
      if (strstr(text, "Wee") != NULL)
      {
        week_logs = true;
        printf("Week\n");
      }
      if (strstr(text, "Mont") != NULL)
      {
        month_logs = true;
        printf("Month\n");
      }
    }

    printf("[Thread %d] Mesajul a fost receptionat...%s\n", tdL.idThread, text);
    char message_response[4096];
    bzero(message_response, sizeof(message_response));
    
    if(security == true){
      security = false;
       char command[100] = "sudo journalctl --file /var/log/journal/remote/remote-";
       strcat(command,ip_adress);
       strcat(command, ".journal");
       strcat(command," -n 20");
       strcat(command, " -u syslog-ng");
       strcat(command, " > logs.txt");

       printf("%s\n", command);

      if (system(command) == -1)
      {
        perror("Eroare la executia comenzii");
        exit(1);
      }
      FILE *file = fopen("logs.txt", "r");
      if (file == NULL)
      {
        perror("Eroare la deschiderea fisierului!\n");
      }
      char continut_fisier[4096];
      continut_fisier[0] = '\0';

      char line[1024];

      while (fgets(line, sizeof(line), file) != NULL)
      {
        strcat(continut_fisier, line);
      }

      fclose(file);
      strcpy(message_response, continut_fisier);

      printf("[Thread %d] Trimitem mesajul inapoi...%s\n", tdL.idThread, message_response);
      if (write(tdL.cl, message_response, sizeof(message_response)) <= 0)
      {
        printf("[Thread %d] ", tdL.idThread);
        perror("[Thread] Eroare la write() catre client.\n");
        break;
      }
      else
      {
        printf("[Thread %d] Mesajul a fost trimis cu succes.\n", tdL.idThread);
      }


    }
    if (ip_primit == true)
    {
      ip_primit = false;
      if (ip_gasit == true)
      {
        strcpy(message_response, "Adresa IP gasita!\n");
      }
      else
      {
        strcpy(message_response, "Adresa IP nu a fost gasita!\n");
      }
      ip_gasit == false;
      printf("[Thread %d] Trimitem mesajul inapoi...%s\n", tdL.idThread, message_response);
      if (write(tdL.cl, message_response, sizeof(message_response)) <= 0)
      {
        printf("[Thread %d] ", tdL.idThread);
        perror("[Thread] Eroare la write() catre client.\n");
        break;
      }
      else
      {
        printf("[Thread %d] Mesajul a fost trimis cu succes.\n", tdL.idThread);
      }
    }
    else if (log_cerut == true)
    {
      char command[100] = "sudo journalctl --file /var/log/journal/remote/remote-"; // De aici construim comanda
      strcat(command, adresa_ip);
      strcat(command, ".journal");

      strcat(command, " -n");

      if (logs_10)
      {
        strcat(command, " 10");
      }
      if (logs_20)
      {
        strcat(command, " 20");
      }
      if (logs_50)
      {
        strcat(command, " 50");
      }
      if (recent_logs)
      {
        strcat(command, " --since='1 day ago'");
      }
      if (today_logs)
      {
        strcat(command, " --since='today'");
      }
      if (week_logs)
      {
        strcat(command, " --since='1 week ago'");
      }
      if (month_logs)
      {
        strcat(command, " --since='1 month ago'");
      }

      if (all_logs)
      {
        // Nu adaug nimic pentru ca toate logurile sunt deja selectate
      }
      else
      {
        if (security_logs)
        {
          strcat(command, " -u syslog-ng");
        }
        if (kernel_logs)
        {
          strcat(command, " -k");
        }
        if (priority_logs)
        {
          strcat(command, " -p 3"); // Aici trebuie sa ajustez
        }
        if (boot_logs)
        {
          strcat(command, " -b");
        }
      }

      printf("Comanda este : %s\n", command); // Aici afisam comanda finala
      // strcpy(message_response,command);
      strcat(command, " > logs.txt");

      if (system(command) == -1)
      {
        perror("Eroare la executia comenzii");
        exit(1);
      }
      FILE *file = fopen("logs.txt", "r");
      if (file == NULL)
      {
        perror("Eroare la deschiderea fisierului!\n");
      }
      char continut_fisier[4096];
      continut_fisier[0] = '\0';

      char line[1024];

      while (fgets(line, sizeof(line), file) != NULL)
      {
        strcat(continut_fisier, line);
      }

      fclose(file);
      strcpy(message_response, continut_fisier);

      printf("[Thread %d] Trimitem mesajul inapoi...%s\n", tdL.idThread, message_response);
      if (write(tdL.cl, message_response, sizeof(message_response)) <= 0)
      {
        printf("[Thread %d] ", tdL.idThread);
        perror("[Thread] Eroare la write() catre client.\n");
        break;
      }
      else
      {
        printf("[Thread %d] Mesajul a fost trimis cu succes.\n", tdL.idThread);
      }
    }
    else if(strstr(text,"doresc") != NULL) // Aici suntem in prima comanda, adica trimtie adresa IP a serverului la interfata utilizatorului, pentru a facilita conexiunea remote a agentului la server
    {
      FILE *fp = popen("ifconfig wlan0", "r");
      if (fp == NULL)
      {
        perror("popen");
      }

      char buffer[128];
      char ip_address[16]; 
      while (fgets(buffer, sizeof(buffer), fp) != NULL)
      {
        if (strstr(buffer, "inet") != NULL)
        {
          if (sscanf(buffer, "%*s %15s", ip_address) == 1)
          {
            printf("Adresa IP a serverului: %s\n", ip_address);
          }
          break;
        }
      }

      pclose(fp);

      strcpy(message_response,ip_address);

      // strcpy(message_response, "Hello ");
      // strcat(message_response, text);
      // strcat(message_response, "!");
      printf("[Thread %d] Trimitem mesajul inapoi...%s\n", tdL.idThread, message_response);

      if (write(tdL.cl, message_response, sizeof(message_response)) <= 0)
      {
        printf("[Thread %d] ", tdL.idThread);
        perror("[Thread] Eroare la write() catre client.\n");
        break;
      }
      else
      {
        printf("[Thread %d] Mesajul a fost trimis cu succes.\n", tdL.idThread);
      }
    }

  }
}
