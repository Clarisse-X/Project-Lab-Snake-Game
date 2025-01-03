#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MAX_PLAYERS 10

typedef struct {
    char username[20];
    int score;
} Player;

Player players[MAX_PLAYERS];
int playerCount = 0;
char map[20][20];
char player = '^', food = '0';
char tail[10] = { '*','*','*','*','*','*','*','*','*','*' };
int y = 12;
int x = 3;
int a[10];
int b[10];
char walk;
Player currentPlayer;

// Function to move the cursor to a specific position
void gotoxy(int x, int y) {
    printf("\033[%d;%dH", y, x);  // ANSI escape code to move the cursor
}

void showMenu();
void playGame();
void showScoreboard();
void updateScoreboard();
void saveScoreboard();
void loadScoreboard();
void createUsername();
void loginPlayer();
void logoutPlayer();
void deletePlayer();
void load(); // Declaring the load function

void createUsername() {
    system("cls");
    printf("=== CREATE YOUR USERNAME ===\n");
    printf("Enter your username: ");
    scanf("%s", currentPlayer.username);
    printf("Username '%s' created successfully!\n", currentPlayer.username);
    sleep(1);
    currentPlayer.score = 0;
	updateScoreboard(); 
}

void loginPlayer() {
    system("cls");
    printf("=== LOGIN ===\n");
    if (playerCount == 0) {
        printf("No users found. You need to create a username.\n");
        createUsername();
    } else {
        char username[20];
        printf("Enter your username: ");
        scanf("%s", username);

        int found = 0;
        int i;
        for (i = 0; i < playerCount; i++) {
            if (strcmp(players[i].username, username) == 0) {
                found = 1;
                strcpy(currentPlayer.username, players[i].username);
                currentPlayer.score = players[i].score;
                printf("Welcome back, %s!\n", currentPlayer.username);
                sleep(1);
                break;
            }
        }

        if (!found) {
            printf("Username not found. Please create a new one.\n");
            sleep(1);
            createUsername();
        }
    }
}

void logoutPlayer() {
    system("cls");
    printf("=== LOGOUT ===\n");
    printf("You have logged out successfully.\n");
    sleep(1);
    loginPlayer();
}

void generatemap(int p, int q, int r) {
    int i, j;
    for (i = 0; i < 20; i++) {
        for (j = 0; j < 20; j++) {
            map[i][j] = ' ';
        }
    }
    map[0][0] = '+';
    map[19][0] = '+';
    map[19][19] = '+';
    map[0][19] = '+';
    for (i = 1; i < 19; i++) {
        map[i][0] = '|';
        map[i][19] = '|';
    }
    for (j = 1; j < 19; j++) {
        map[0][j] = '=';
        map[19][j] = '=';
    }
    map[y][x] = player;
    for (i = 0; i < r; i++) {
        map[a[i]][b[i]] = tail[i];
    }
    map[p][q] = food;
}

void outputMap(int score) {
    int i, j;
    for (i = 0; i < 20; i++) {
        for (j = 0; j < 20; j++) {
            printf("%c", map[i][j]);
        }
        printf("\n");
    }
    printf("Score: 00%d\n", score);
    printf("Player: %s\n", currentPlayer.username); 
}

void load() {
    int r, q;
    gotoxy(36, 14);  // Now works due to custom gotoxy implementation
    printf("loading...");
    gotoxy(30, 15);  // same here
    for (r = 1; r <= 20; r++)
    {
        for (q = 0; q <= 100000000; q++)
            ; // to display the character slowly
        printf("%c", 177);  // 177 is the block character
    }
    getch();
}

void playGame() {
    int i, j, n = 0, p = 10, q = 10, rand = 2, r = 1;
    currentPlayer.score = 0;
    y = 12;
    x = 3;

    system("color 3");
    load();  // Add this line to show the loading screen before starting the game

    do {
        system("cls");
        generatemap(p, q, r);
        outputMap(currentPlayer.score);

        // Periksa apakah kepala ular menabrak tubuhnya sendiri
        for (i = 1; i < r; i++) {
            if (a[i] == y && b[i] == x) {
                system("cls");
                printf("\t\t+==============================+\n");
                printf("\t\t       GAME OVER!!!            \n");
                printf("\t\t+==============================+\n");
                printf("\tYour Score: %d\n", currentPlayer.score);
                printf("\nPress any key to return to menu...");
                getch();
                updateScoreboard();
                saveScoreboard();
                showMenu();
                return; // Keluar dari fungsi playGame
            }
        }

        walk = getch();
        for (i = 9; i > 0; i--) {
            a[i] = a[i - 1];
            b[i] = b[i - 1];
        }
        a[0] = y; b[0] = x;
        switch (walk) {
        case 'w':
            if (map[y - 1][x] == ' ') {
                y--;
                map[y][x] = player;
            }
            else if (map[y - 1][x] == food) {
                p *= (rand + y * 3); q *= (rand + x * 9);
                if (p >= 19) p %= 13; p++;
                if (q >= 19) q %= 13; q++;
                currentPlayer.score++;
                r++;
            }
            player = '^';
            break;
        case 's':
            if (map[y + 1][x] == ' ') {
                y++;
                map[y][x] = player;
            }
            else if (map[y + 1][x] == food) {
                p *= (rand + y * 6); q *= (rand + x * 8);
                if (p >= 19) p %= 18; p++;
                if (q >= 19) q %= 15; q++;
                currentPlayer.score++;
                r++;
            }
            player = 'v';
            break;
        case 'a':
            if (map[y][x - 1] == ' ') {
                x--;
                map[y][x] = player;
            }
            else if (map[y][x - 1] == food) {
                p *= (rand + y * 5); q *= (rand + x * 7);
                if (p >= 19) p %= 18; p++;
                if (q >= 19) q %= 17; q++;
                currentPlayer.score++;
                r++;
            }
            player = '<';
            break;
        case 'd':
            if (map[y][x + 1] == ' ') {
                x++;
                map[y][x] = player;
            }
            else if (map[y][x + 1] == food) {
                p *= (rand + y * 2); q *= (rand + x * 3);
                if (p >= 19) p %= 18; p++;
                if (q >= 19) q %= 18; q++;
                currentPlayer.score++;
                r++;
            }
            player = '>';
            break;
        }
    } while (currentPlayer.score < 10);

    system("cls");
    printf("\t\t+==============================+\n");
    printf("\t\t       CONGRATULATIONS!!!        \n");
    printf("\t\t+==============================+\n");
    printf("\tYour Score: 10\n");
    printf("\nPress any key to return to menu...");
    getch();
    
    updateScoreboard();
    saveScoreboard();
    showMenu();
}

void saveScoreboard() {
    FILE *file = fopen("scoreboard.txt", "w"); //path, mode
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }
	int i;
    for (i = 0; i < playerCount; i++) {
        fprintf(file, "%s#%d\n", players[i].username, players[i].score);
    }

    fclose(file);
    printf("Scoreboard saved successfully!\n");
}

void loadScoreboard() {
    FILE *file = fopen("scoreboard.txt", "r"); //path, mode
    if (file == NULL) {
        printf("No saved scoreboard found, starting fresh.\n");
        return;
    }

    while (fscanf(file, "%[^#]#%d\n", players[playerCount].username, &players[playerCount].score) == 2) {
        playerCount++;
        if (playerCount >= MAX_PLAYERS) break;
    }

    fclose(file);
    printf("Scoreboard loaded successfully!\n");
}

void updateScoreboard() {
	int found = 0;
	int i, j;
	for (i = 0; i < playerCount; i++) {
        if (strcmp(players[i].username, currentPlayer.username) == 0) {
            players[i].score = currentPlayer.score; //Update score
            found = 1;
            break;
        }
    }
	
	if (!found && playerCount < MAX_PLAYERS) {
        players[playerCount++] = currentPlayer;
    } else {
        printf("Scoreboard is full. Cannot add more scores.\n");
    }

    //Mengurutkan player berdasarkan score (descending)
    for (i = 0; i < playerCount - 1; i++) {
        for (j = 0; j < playerCount - 1; j++) {
            if (players[i].score < players[j].score) {
                Player temp = players[i];
                players[i] = players[j];
                players[j] = temp;
            }
        }
    }
}

void showScoreboard() {
    system("cls");
    printf("\t+============================+\n");
    printf("\t|        SCOREBOARD           |\n");
    printf("\t+============================+\n");
    int i;
    for (i = 0; i < playerCount; i++) {
        printf("\t| %d. %s - %d\n", i + 1, players[i].username, players[i].score);
    }
    printf("\t+============================+\n");
    
    char choice;
    printf("Do you want to search for a username? (y/n): ");
    scanf(" %c", &choice);

    if (choice == 'y' || choice == 'Y') {
        char username[20];
        printf("Enter the username to search: ");
        scanf("%s", username);

        int found = 0;
        int i;
        for (i = 0; i < playerCount; i++) {
            if (strcmp(players[i].username, username) == 0) {
                printf("\nUsername: %s\nScore: %d\n", players[i].username, players[i].score);
                found = 1;
                break;
            }
        }

        if (!found) {
            printf("\nUsername '%s' not found in the scoreboard.\n", username);
        }
    }
    
    printf("Press any key to go back to menu...");
    getch();
}

void deletePlayer() {
	system("cls");
	int i, j;
	printf("\t+============================+\n");
    printf("\t|        SCOREBOARD           |\n");
    printf("\t+============================+\n");
    for (i = 0; i < playerCount; i++) {
        printf("\t| %d. %s - %d\n", i + 1, players[i].username, players[i].score);
    }
    printf("\t+============================+\n");
    
	char username[20];
    printf("Enter the username to delete: ");
    scanf("%s", username);
    
    int found = 0;
    for (i = 0; i < playerCount; i++) {
        if (strcmp(players[i].username, username) == 0) {
            for (j = i; j < playerCount - 1; j++) {
                players[j] = players[j + 1];
            }
            playerCount--;
            found = 1;
            printf("Player '%s' deleted successfully!\n", username);
            sleep(3);
            break;
        }
    }

    if (!found) {
        printf("Player '%s' not found in the scoreboard.\n", username);
    }

    saveScoreboard();
}

void showMenu() {
    int choice;
    do {
        system("cls");
        printf("\t+============================+\n");
        printf("\t|         SNAKE GAME          |\n");
        printf("\t+============================+\n");
        printf("\t|  1. Play                    |\n");
        printf("\t|  2. Scoreboard              |\n");
        printf("\t|  3. Logout                  |\n");
        printf("\t|  4. Delete Account          |\n");
        printf("\t|  5. Exit                    |\n");
        printf("\t+============================+\n");
        printf("\tChoose an option (1-5): ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            system("cls");
            printf("Controls: Use W (up), A (left), S (down), D (right) to move.\n");
            printf("Press any key to start the game...");
            getch();
            playGame();
            break;
        case 2:
            showScoreboard();
            break;
        case 3:
            logoutPlayer();
            break;
        case 4:
            deletePlayer();
            break;
        case 5:
            exit(0);
        default:
            printf("Invalid choice. Returning to menu in 3 seconds...\n");
            sleep(3); 
            break;
        }
    } while (choice != 5);
}

int main() {
	loadScoreboard();
	loginPlayer();
    showMenu();
    saveScoreboard();
    return 0;
} 
