#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

//butuh file processing untuk edit nodenya
struct TimeS
{
   char day[40];
   double time_long;
   char week[13];
   char year[13];
};

struct comment_like
{
   char comment[260];
   char get_time[260];
   comment_like *next,*prev;
};

struct binTrash
{
   char temp[260][260];
   char validate;
   binTrash *next,*prev;
};

struct Notes
{
   char Note[260][260];//since note can taken a paragraph
   comment_like *head,*tail;//harena setiap node ada link list comment_likenya
   Notes *next,*prev;//for make a link list notes
};

struct message
{
   char word[260][260];
};

struct Send_Request
{
   
};

struct Inbox
{

};

struct FriendInbox{
   char username[24+1];
   FriendInbox *next_inbox, *previous_inbox;
};

struct FriendRequested{
   char username[24+1];
   FriendRequested *next_requested, *previous_requested;
};

struct FriendList{
   char username[24+1];
   FriendList *next_friend, *previous_friend;
};

struct UserList
{
   char username[260]; //Bukannya 24 ya paling panjang length-nya?
   char password[260]; //Bukannya 24 ya paling panjang length-nya?
   struct FriendInbox friend_inbox;
   struct FriendRequested friend_requested; //struct-nya dihilangkan juga gpp (pake .cpp soalnya)
   struct FriendList friend_list;
   UserList *next_user, *previous_user;
   Notes *head, *tail;
   binTrash *test1, *test2;
};


Notes*makeNotes(Notes s);


void edit_note();
void main_menu();
void make_account();
void get_login();
void pushMid();//using struct parameter
void pushHead();
void pushTail();

void make_account()
{

}

void show_users(UserList **first, UserList **last){
   UserList *current = *first;
   int number = 1;
   while(current != NULL){
      printf("%-3d %s\n", number++, current->username);
      current = current->next_user;
   }
   // UserList *current = *last;
   // while(current != NULL){
   //     printf("%s\n", current->username);
   //     current = current->previous_user;
   // }
   puts(""); //Debug
}

UserList *createNew(char username[], char password[]){
   UserList *newUser = (UserList *) malloc(sizeof(UserList));
   strcpy(newUser->username, username);
   strcpy(newUser->password, password);
   newUser->next_user = newUser->previous_user = NULL;

   strcpy(newUser->friend_inbox.username, username);
   newUser->friend_inbox.next_inbox = newUser->friend_inbox.previous_inbox = NULL;
   strcpy(newUser->friend_requested.username, username);
   newUser->friend_requested.next_requested = newUser->friend_requested.previous_requested = NULL;
   strcpy(newUser->friend_list.username, username);
   newUser->friend_list.next_friend = newUser->friend_list.previous_friend = NULL;
   return newUser;
}

void register_user(UserList **first, UserList **last, char username[], char password[]){
   // puts("Berhasil register1"); //Debug
   UserList *newUser = createNew(username, password);
   // puts("Berhasil register2"); //Debug
   if((*first) == NULL){
      // puts("Berhasil register if1"); //Debug
      (*first) = (*last) = newUser;
   }
   else if(strcmp(username, (**first).username) < 0){
      // puts("Berhasil register if2"); //Debug
      newUser->next_user = (*first);
      (*first)->previous_user = newUser;
      (*first) = newUser;
   }
   else if(strcmp(username, (**last).username) > 0){
      // puts("Berhasil register if3"); //Debug
      (**last).next_user = newUser;
      newUser->previous_user = (*last);
      (*last) = newUser;
   }
   else{
      // puts("Berhasil register if4"); //Debug
      UserList *current = (*first);
      while(current != NULL){
         if(strcmp(current->username, username) >= 0){
               break;
         }
         else{
               current = current->next_user;
         }
      }
      // printf("current: %s\n", current->username); //Debug

      // show_users(first, last); //Debug

      // printf("newUser->previous_user: %s\n", current->previous_user); //Debug
      newUser->previous_user = current->previous_user;
      // printf("newUser->next_user: %s\n", current->username); //Debug
      newUser->next_user = current;
      // printf("current->previous_user: %s\n", newUser->username); //Debug
      current->previous_user = newUser;
      // printf("newUser->previous_user->next_user: %s\n", newUser->username); //Debug
      newUser->previous_user->next_user = newUser;
   }
}

void menu(UserList **first, UserList **last)
{
   puts("Oo=====================================oO");
   puts("               STUDY NETWORK             ");
   puts("Oo=====================================oO\n");
   puts("[All User]\n");
   show_users(first, last);
   puts("------------------------");
   puts("[1] Register");
   puts("[2] Login");
   puts("[3] Exit");
   puts("------------------------");
   puts("------------------------");
   puts("Press 0 and enter to abort an operation");
   puts("------------------------");
   printf(">> ");
   char use = '\0';
   while(use<'0'||use>'3')
   {
      scanf("%c",&use);
      getchar();
   }
   
   if(use=='0')
   {
      // puts("back to main menu");
      menu(first, last);
   }
   else if (use=='1')
   {
      char name[24+1], pass[24+1];
      puts("make account in progress");
      printf("Username: ");
      scanf("%[^\n]", name);
      getchar();
      printf("Password: ");
      scanf("%[^\n]", pass);
      getchar();
      register_user(first, last, name, pass);
   }
   else if (use=='2')
   {
      // puts("wait, our login system in progress");
      // get_login();
      
   }
   else if (use=='3')
   {
      system("cls||clear");
      // puts("we gonna exit this app");
   }

   // printf("YOU MADE A MAIN MENU !! :>");

}

int main(){
   UserList *first = NULL;
   UserList *last = NULL; //User List diurutkan sesuai namanya. Contohnya first di inisial A, sedangkan last di inisial Z
   char flag;

for(;;){
   menu(&first, &last);
   printf("Enter to Continue...(0 to exit program)");
   scanf("%c", &flag);
   if(flag == '0'){
      break;
   }
}

   return 0;
}
