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

struct FriendInbox{ //Sebagai tempat message ketika ada yang request friend ke user
    char username[24+1];
    FriendInbox *next_inbox, *previous_inbox;
};

struct FriendRequested{ //Sebagai tempat message user me-request friend ke siapa saja
    char username[24+1];
    FriendRequested *next_requested, *previous_requested;
};

struct FriendList{ //Sebagai Database daftar teman User
    char username[24+1];
    FriendList *next_friend, *previous_friend;
};

struct UserList //sebagai Database daftar User te-registrasi
{
    char username[260]; //Bukannya 24 ya paling panjang length-nya?
    char password[260]; //Bukannya 24 ya paling panjang length-nya?
    
    struct FriendInbox friend_inbox; //tiap user ada database friend inbox, friend requested, dan friend list nya tersendiri
    // FriendInbox *head_friend_inbox;
    struct FriendRequested friend_requested; //struct-nya dihilangkan juga gpp (pake .cpp soalnya)
    // FriendRequested *head_friend_requested;
    struct FriendList friend_list;
    // FriendList *head_friend_list;
    
    UserList *next_user, *previous_user; //next user untuk ke user selanjutnya di daftar user
    FriendList *first_friend, *last_friend; //same as first and last of UserList in "int main()"
    Notes *head, *tail;
    binTrash *test1, *test2;
};


Notes*makeNotes(Notes s);


void edit_note();
void main_menu();
void make_account();
void pushMid();//using struct parameter
void pushHead();
void pushTail();

void make_account()
{

}

void show_users(UserList **first, UserList **last){ //menampilkan semua user yang telah te-registrasi
    UserList *current = *first;
    int number = 1;
    puts("[All User]\n");
    puts("No. Username");
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

UserList *createNew(char username[], char password[]){ //buat user baru
    UserList *newUser = (UserList *) malloc(sizeof(UserList));
    strcpy(newUser->username, username); //inisialisasi username dan password
    strcpy(newUser->password, password);
    newUser->next_user = newUser->previous_user = NULL; //semua pointer yang ada di user baru diarahkan ke NULL

    strcpy(newUser->friend_inbox.username, username);
    newUser->friend_inbox.next_inbox = newUser->friend_inbox.previous_inbox = NULL;
    strcpy(newUser->friend_requested.username, username);
    newUser->friend_requested.next_requested = newUser->friend_requested.previous_requested = NULL;
    strcpy(newUser->friend_list.username, username);
    newUser->friend_list.next_friend = newUser->friend_list.previous_friend = NULL;
    newUser->first_friend = newUser->last_friend = NULL;
    return newUser;
}

void register_user(UserList **first, UserList **last, char username[], char password[]){
    // puts("Berhasil register1"); //Debug
    UserList *newUser = createNew(username, password);
    // puts("Berhasil register2"); //Debug
    if((*first) == NULL){ //Jika belum ada user te-registrasi (first masi mengarah ke NULL)
        // puts("Berhasil register if1"); //Debug
        (*first) = (*last) = newUser;
    }
    else if(strcmp(username, (**first).username) < 0){ //pushHead
        // puts("Berhasil register if2"); //Debug
        newUser->next_user = (*first);
        (*first)->previous_user = newUser;
        (*first) = newUser;
    }
    else if(strcmp(username, (**last).username) > 0){ //pushTail
        // puts("Berhasil register if3"); //Debug
        (**last).next_user = newUser;
        newUser->previous_user = (*last);
        (*last) = newUser;
    }
    else{ //pushMid
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

//untuk mengecek apakah username dan password sudah benar atau belum
char get_login(const char *temp_name, const char *temp_pass, UserList **first, UserList **logged_user){
    UserList *current = (*first);
    while(current != NULL){ //Jika current-nya NULL, berarti user belum te-registrasi
        if(strcmp(current->username, temp_name) < 0){
            current = current->next_user;
        }
        else if(strcmp(current->username, temp_name) == 0){ //Jika dapat username yang sesuai
            if(strcmp(current->password, temp_pass) == 0){ //Jika password benar
                // puts("berhasil"); //Debug
                (*logged_user) = current;
                return '0';
            }
            else{ //Jika password salah
                puts("\nUsername or Password is wrong!");
                return '1';
            }
        }
        else{ //Username belum te-registrasi
            puts("\nUsername didn't exist!");
            return '1';
        }
    }
    if(current == NULL){
        puts("\nUsername didn't exist!");
        return '1';
    }
}

void show_friend(UserList **logged_user){ //untuk menampilkan semua teman User
    FriendList *current = (**logged_user).first_friend;
    int number = 1;
    printf("[All Friends of %s]\n", (**logged_user).username);
    puts("No. Username");
    while(current != NULL){
        printf("%-3d %s\n", number++, current->username);
        current = current->next_friend;
    }
}

void pushFriend(UserList **currentUser, FriendList **newFriend){ //Tambah Friend List
    // printf("%s\n", (**currentUser).username); //Debug
    if((*currentUser)->first_friend == NULL){ //friend list masi kosong
        puts("if"); //Debug
        (*currentUser)->first_friend = (*currentUser)->last_friend = (*newFriend);
        (*currentUser)->friend_list.next_friend = (*newFriend);
        // (*newFriend)->previous_friend = (*currentUser)->friend_list; // belum yakin////////////
        //belum dibuat null first_friendnya
    }
    else if(strcmp((*newFriend)->username, (*currentUser)->first_friend->username) < 0){ //pushHead
        puts("else if 1"); //Debug
        (*currentUser)->first_friend->previous_friend = (*newFriend);
        (*newFriend)->next_friend = (*currentUser)->first_friend;
        (*currentUser)->first_friend = (*newFriend);
        // (*currentUser)->friend_list.next_friend = (*newFriend);
    }
    else if(strcmp((*newFriend)->username, (*currentUser)->last_friend->username) > 0){ //pushTail
        puts("else if 2"); //Debug
        (*currentUser)->last_friend->next_friend = (*newFriend);
        (*newFriend)->previous_friend = (*currentUser)->last_friend;
        (*currentUser)->last_friend = (*newFriend);
    }
    else{ //pushMid
        puts("else"); //Debug
        FriendList *check_current = (*currentUser)->first_friend;
        while(check_current != NULL){
            if(strcmp((*newFriend)->username, check_current->username) < 0){
                break;
            }
            check_current = check_current->next_friend;
        }
        (*newFriend)->next_friend = check_current;
        (*newFriend)->previous_friend = check_current->previous_friend;
        check_current->previous_friend->next_friend = (*newFriend);
        check_current->previous_friend = (*newFriend);
    }
}

void add_friend(UserList **logged_user, UserList **first, const char *name){ //menambahkan friend list

    // menambahkan friend list di user yang melakukan request (yang di-accept)
    UserList *currentUser = (*first); //currentUser adalah user yang melakukan add request
    while(currentUser != NULL){
        if(strcmp(currentUser->username, name) == 0){
            FriendList *newFriend = (FriendList *) malloc(sizeof(FriendList));
            newFriend->next_friend = newFriend->previous_friend = NULL;
            strcpy(newFriend->username, (**logged_user).username);
            // puts("go to push1"); //Debug
            pushFriend(&currentUser, &newFriend); //Tambah Friend List
            break;
        }
        currentUser = currentUser->next_user;
    }

    //menambahkan friend list di user yang di-request (yang meng-accept)
    FriendList *newFriend = (FriendList *) malloc(sizeof(FriendList));
    newFriend->next_friend = newFriend->previous_friend = NULL;
    strcpy(newFriend->username, currentUser->username);
    // puts("go to push2"); //Debug
    pushFriend(logged_user, &newFriend); //Tambah Friend List
}

void delete_inbox_request(FriendInbox **current, UserList **logged_user){
    if((*current)->previous_inbox == NULL && (*current)->next_inbox == NULL){ //Jika hanya ada 1 pesan/request
        (**logged_user).friend_inbox.next_inbox = NULL;
    }
    else if((*current)->previous_inbox == NULL){ //Jika pesan yang dihapus adalah yang paling atas/kiri/terbaru
        (**logged_user).friend_inbox.next_inbox = (*current)->next_inbox;
        (*current)->next_inbox->previous_inbox = (*current)->next_inbox = NULL;
    }
    else if((*current)->next_inbox == NULL){ //Jika pesan yang dihapus adalah yang paling bawah/kanan/terlama
        (*current)->previous_inbox->next_inbox = (*current)->previous_inbox = NULL;
    }
    else{ //Jika pesan yang dihapus berada ditengah-tengah
        (*current)->next_inbox->previous_inbox = (*current)->previous_inbox;
        (*current)->previous_inbox->next_inbox = (*current)->next_inbox;
        (*current)->next_inbox = (*current)->previous_inbox = NULL;    
    }
    free((*current));
}

void check_user_inbox(UserList **logged_user, const char *name, UserList **first){
    //cek nama di dalam inbox, jika ada maka accept, jika tidak ada maka skip

    FriendInbox *current = (**logged_user).friend_inbox.next_inbox;
    while(current != NULL){
        if(strcmp(current->username, name) == 0){ //yg mau di accept ditemukan di inbox
            printf("%s accepted as friend.\n", current->username);
            add_friend(logged_user, first, name); //menambahkan friend list

            //pesan bersangkutan yang ada di-inbox mau dihapus
            delete_inbox_request(&current, logged_user);
            // if(current->previous_inbox == NULL && current->next_inbox == NULL){ //Jika hanya ada 1 pesan/request
            //     // puts("if"); //Debug
            //     (**logged_user).friend_inbox.next_inbox = NULL;
            // }
            // else if(current->previous_inbox == NULL){ //Jika pesan yang dihapus adalah yang paling atas/kiri/terbaru
            //     // puts("else if1"); //Debug
            //     (**logged_user).friend_inbox.next_inbox = current->next_inbox;
            //     current->next_inbox->previous_inbox = current->next_inbox = NULL;
            // }
            // else if(current->next_inbox == NULL){ //Jika pesan yang dihapus adalah yang paling bawah/kanan/terlama
            //     // puts("else if2"); //Debug
            //     current->previous_inbox->next_inbox = current->previous_inbox = NULL;
            // }
            // else{ //Jika pesan yang dihapus berada ditengah-tengah
            //     // puts("else"); //Debug
            //     current->next_inbox->previous_inbox = current->previous_inbox;
            //     current->previous_inbox->next_inbox = current->next_inbox;
            //     current->next_inbox = current->previous_inbox = NULL;    
            // }
            // free(current);
            // // puts("test5"); //Debug
            return;
        }
        current = current->next_inbox;
    }
    if(current == NULL){ //Jika sampai disini, maka user yang di accept tidak ditemukan di inbox
        puts("The username didn't ask friend request to you!");
        printf("Press Enter to Continue...");
        getchar();
    }
}


void respond_inbox(UserList **logged_user, UserList **first){ //meresponi friend request di inbox
    char name[24+1];
    puts("What the username of user do you want to accept? (Type \'-\' if there's no one)");
    printf(">> ");
    scanf("%[^\n]", name);
    getchar();
    if(strcmp(name, "-") != 0){
        //cek yang di accept, jika ada maka accept, jika tidak ada maka skip
        check_user_inbox(logged_user, name, first);
    }

    puts("What the username of user do you want to decline? (Type \'-\' if there's no one)");
    printf(">> ");
    scanf("%[^\n]", name);
    getchar();
    if(strcmp(name, "-") != 0){
        FriendInbox *current = (**logged_user).friend_inbox.next_inbox;
        while(current != NULL){
            if(strcmp(current->username, name) == 0){ //jika nama yang di decline ada di inbox
                printf("%s declined as friend.\n", current->username);

                //menghapus nama di decline di inbox
                delete_inbox_request(&current, logged_user);
                return;
            }
            current = current->next_inbox;
        }
        if(current == NULL){ //jika nama yang di didecline tidak ada di inbox
            puts("The username didn't ask friend request to you!");
            return;
        }
    }
}

void show_friend_inbox(UserList **logged_user, UserList **first){
    //menampilkan isi inbox sekaligus melakukan proses accept/decline/pending

    //menampilkan isi inbox
    puts("------------------------");
    printf("[All Friend Requests to %s]\n", (**logged_user).username);
    puts("No. Username");
    FriendInbox *currentInbox = (**logged_user).friend_inbox.next_inbox;
    for(int i=1; currentInbox != NULL; i++){
        printf("%-3d %s\n", i, currentInbox->username);
        currentInbox = currentInbox->next_inbox;
    }
    puts("\n------------------------");

    //meresponi friend request yang masuk di inbox
    respond_inbox(logged_user, first);

    printf("Press Enter to Continue...");
    getchar();
}

void show_sent(UserList **logged_user){ //menampilkan isi sent request
    puts("------------------------");
    printf("[All Send Requests of %s]\n", (**logged_user).username);
    puts("No. Username");
    FriendRequested *currentSent = (**logged_user).friend_requested.next_requested;
    for(int i=1; currentSent != NULL; i++){
        printf("%-3d %s\n", i, currentSent->username);
        currentSent = currentSent->next_requested;
    }
    puts("\n------------------------");
    printf("Press Enter to Continue...");
    getchar();
}

void add_request(UserList **userCurrent, UserList **logged_user){ //Menggunakan pushHead

    //Menambahkan request ke dalam inbox yang di-add
    FriendInbox *newInbox = (FriendInbox *) malloc(sizeof(FriendRequested));
    strcpy(newInbox->username, (**logged_user).username);
    newInbox->next_inbox = newInbox->previous_inbox = NULL;

    if((**userCurrent).friend_inbox.next_inbox == NULL){ //jika inbox kosong
        (**userCurrent).friend_inbox.next_inbox = newInbox;
        // newInbox->previous_inbox = (**userCurrent).friend_inbox.next_inbox;//////////////////////
    }
    else{ //jika inbox sudah ada isinya
        (**userCurrent).friend_inbox.next_inbox->previous_inbox = newInbox;
        newInbox->next_inbox = (**userCurrent).friend_inbox.next_inbox;
        (**userCurrent).friend_inbox.next_inbox = newInbox;
        // newInbox->previous_inbox = (**userCurrent).friend_inbox.next_inbox;
    }

    //Menambahkan request ke dalam sent request yang meng-add
    FriendRequested *newRequest = (FriendRequested *) malloc(sizeof(FriendRequested));
    strcpy(newRequest->username, (**userCurrent).username);
    newRequest->next_requested = newRequest->previous_requested = NULL;

    if((**logged_user).friend_requested.next_requested == NULL){ //jika sent request kosong
        (**logged_user).friend_requested.next_requested = newRequest;
        // newRequest->previous_requested = (**logged_user).friend_requested.next_requested;
    }
    else{ //jika sent request ada isinya
        (**logged_user).friend_requested.next_requested->previous_requested = newRequest;
        newRequest->next_requested = (**logged_user).friend_requested.next_requested;
        (**logged_user).friend_requested.next_requested = newRequest;
        // newRequest->previous_requested = (**logged_user).friend_requested.next_requested;
    }
}

char check_friend_inbox(UserList **userCurrent, const char *logged_username){
    //buat cek apakah yang di-add telah di add sebelumnya atau belum (untuk menghindari spam dalam add)

    FriendInbox *current = (**userCurrent).friend_inbox.next_inbox;
    while(current != NULL){
        if(strcmp(current->username, logged_username) == 0){
            puts("Can't add user, your last request didn't responded yet by the user!");
            return '1';
        }
        current = current->next_inbox;
    }
    if(current == NULL){
        return '0';
    }
}

void search_user(UserList **first, UserList **last, const char *name, UserList **logged_user){
    if(strcmp((**logged_user).username, name) == 0){ //ga boleh add diri sendiri sebagai teman
        puts("You can't add your own self as friend!");
        return;
    }
    UserList *userCurrent = (*first); //userCurrent adalah username yang mau di add sebagai teman
    while(userCurrent != NULL){
        if(strcmp(userCurrent->username, name) == 0){ //Username-nya ditemukan (bisa di-add)
            char flag = check_friend_inbox(&userCurrent, (**logged_user).username);
            //buat cek apakah yang di-add telah di add sebelumnya atau belum (untuk menghindari spam dalam add)

            if(flag == '1'){ //Jika sudah pernah add sebelumnya dan belum ditanggapi oleh yang di-add
                return;
            }

            FriendList *friendCurrent = userCurrent->first_friend;
            while(friendCurrent != NULL){ //Cek apakah yang di-add telah menjadi teman atau belum
                if(strcmp(friendCurrent->username, name) == 0){ //Sudah jadi teman
                    puts("The User Already Added in Friend List!");
                    return;
                }
                friendCurrent = friendCurrent->next_friend;
            }
            if(friendCurrent == NULL){ //Belum jadi teman
                add_request(&userCurrent, logged_user); //masukkan ke inbox dan sent request
                puts("Success.. Waiting for added friend to accept.");
                return;
            }
        }
        else if(strcmp(userCurrent->username, name) > 0){ //Usernamenya ga ada
            break;
        }
        userCurrent = userCurrent->next_user;
    }

    //Jika sampai masuk kesini, berarti username yang diinput tidak pernah diregister sebelumnya
    puts("The username didn't exist!");
    return;
}

void login_menu(UserList **logged_user, UserList **first, UserList **last){ //Menu setelah User berhasil login
    for(;;){
    system("CLS || clear");
    puts("Oo=====================================oO");
    printf("Welcome Back, %s!\n", (*logged_user)->username);
    puts("Oo=====================================oO");
    //Logged in: [Waktu];
    puts("------------------------\n");
    show_friend(logged_user);

    puts("\n------------------------");
    puts("       >> Menu <<         ");
    puts("------------------------");
    puts("[1] Add Friend");
    puts("[2] Remove Friend");
    puts("[3] View Inbox");
    puts("[4] View Sent Request");
    puts("[5] Add / Edit / Announce / Delete Note");
    puts("[6] Log out");
    puts("------------------------");
    char use = '\0';
    while(use<'1' || use>'6')
    {
        printf(">> ");
        scanf("%c",&use);
        getchar();
    }
    switch(use){
        case '1':
            // puts("[1] Add Friend");
            puts("------------------------");
            show_users(first, last);
            
            puts("\nType down the username of user do you want to add:\n");
            printf(">> ");
            char name[24+1];
            scanf("%[^\n]", name);
            getchar();
            search_user(first, last, name, logged_user); //search nama user yang mau di-add
            printf("Press Enter to Continue...");
            getchar();
            break;

        case '2':
            // puts("[2] Remove Friend");
            break;
            
        case '3':
            show_friend_inbox(logged_user, first); //menampilkan isi inbox
            break;

        case '4':
            show_sent(logged_user); //menampilkan isi sent request
            break;
            
        case '5':
            // puts("[5] Add / Edit / Announce / Delete Note");
            break;
            
        case '6':
            // puts("[6] Log out");
            return;
            
        default: //Debug
            printf("[PROBLEM IN CODE]");
    }
   
    }
}

void menu(UserList **first, UserList **last) //Menu awal ketika program baru berjalan
{for(;;){
    system("CLS || clear");
    puts("Oo=====================================oO");
    puts("               STUDY NETWORK             ");
    puts("Oo=====================================oO\n");
    show_users(first, last);
    puts("------------------------");
    puts("[1] Register");
    puts("[2] Login");
    puts("[3] Exit");
    puts("------------------------");
    puts("------------------------");
    puts("Press 0 and enter to abort an operation");
    puts("------------------------");
    char use = '\0';
    while(use<'0'||use>'3')
    {
        printf(">> ");
        scanf("%c",&use);
        getchar();
    }
    
    if(use=='0')
    {
        // puts("back to main menu");
        menu(first, last);
    }
    else if (use=='1') //BELUM ADA VALIDASI REGISTER
    {
        char name[24+1], pass[24+1];
        puts("make account in progress");
        printf("Username: ");
        scanf("%[^\n]", name);
        getchar();
        printf("Password: ");
        scanf("%[^\n]", pass);
        getchar();
        if(strcmp(name, "-") != 0){ //ga boleh "-", karena nanti tanda "-" akan dipakai sebagai flag
            register_user(first, last, name, pass);
        }
        else{
            puts("Invalid Username!");
            printf("Press Enter to Continue...");
            getchar();
        }
    }
    else if (use=='2')
    {
        puts("------------------------");
        printf("Username: ");
        char temp_name[24+1], temp_pass[24+1];
        scanf("%[^\n]", temp_name);
        getchar();
        printf("Password: ");
        scanf("%[^\n]", temp_pass);
        getchar();
        
        UserList *logged_user; //Sebagai penanda "user yang login" di dalam "daftar user"
        char flag = get_login(temp_name, temp_pass, first, &logged_user); //cek username dan pass (Validasi)
        if(flag == '0'){ //login sukses
            puts("\n--- Login Successfull ---");
            printf("Press Enter to Continue...");
            getchar();
            login_menu(&logged_user, first, last); //masuk ke menu login
        }
        else if(flag == '1'){ //login gagal
            // puts("else if '1' login"); //Debug
            printf("Press Enter to Continue...");
            getchar();
            continue; //kembali ke awal
        }
        else{ //Jika masuk kesini, berarti ada kesalahan dalam coding
            puts("[PROBLEM IN CODE]"); //Debug
            printf("Press Enter to Continue...");
            getchar();
            continue; //kembali ke awal
        }
    }
    else if (use=='3') //Program berakhir
    {
        system("cls||clear");
        return; //keluar dari modul menu
        // puts("we gonna exit this app");
    }

    // printf("YOU MADE A MAIN MENU !! :>");

}
   
}

int main(){
    UserList *first = NULL; //first paling kiri/awal, last paling kanan/akhir
    UserList *last = NULL; //User List diurutkan sesuai namanya. Contohnya first di inisial A, sedangkan last di inisial Z
    menu(&first, &last);
    return 0;
}