#include <stdio.h>
#include <string.h>

typedef struct {
    char lastName[50];
    char firstName[50];
    char middleName[50];
    char address[100];
    char contact[20];
    char details[200];
    int id;
    char status[10];
} Appointment;

typedef struct {
    char username[50];
    char password[50];
} User;

Appointment appointments[100];
User users[100];
int currentAppointment = 0;
int currentUser = 0;

void adminDashboard();
void userDashboard(int currentUserIndex);
void signUp();
void signIn();
void bookAppointment(int currentUserIndex);
void saveAppointmentsToFile();
void loadAppointmentsFromFile();
void saveUsersToFile();
void loadUsersFromFile();


int main() {
    loadAppointmentsFromFile();
    loadUsersFromFile();

    int choice;
    while (1) {
        printf("\n----- ^_^ Appointment System ^_^ -----\n");
        printf("1. Sign In\n2. Sign Up\n3. Exit\nChoice: ");
        scanf("%d", &choice);
        getchar();

        if (choice == 1) signIn();
        else if (choice == 2) signUp();
        else if (choice == 3) break;
        else printf("Invalid choice!\n");
    }
    return 0;
}

void saveAppointmentsToFile() {
    FILE *fp = fopen("appointments.txt", "w");
    if (fp == NULL) return;
    for (int i = 0; i < currentAppointment; i++) {
        fprintf(fp, "%d|%s|%s|%s|%s|%s|%s|%s\n",
                appointments[i].id,
                appointments[i].lastName,
                appointments[i].firstName,
                appointments[i].middleName,
                appointments[i].address,
                appointments[i].contact,
                appointments[i].details,
                appointments[i].status);
    }
    fclose(fp);
}

void loadAppointmentsFromFile() {
    FILE *fp = fopen("appointments.txt", "r");
    if (fp == NULL) return;
    while (fscanf(fp, "%d|%49[^|]|%49[^|]|%49[^|]|%99[^|]|%19[^|]|%199[^|]|%9[^\n]\n",
                  &appointments[currentAppointment].id,
                  appointments[currentAppointment].lastName,
                  appointments[currentAppointment].firstName,
                  appointments[currentAppointment].middleName,
                  appointments[currentAppointment].address,
                  appointments[currentAppointment].contact,
                  appointments[currentAppointment].details,
                  appointments[currentAppointment].status) == 8) {
        currentAppointment++;
    }
    fclose(fp);
}

void saveUsersToFile() {
    FILE *fp = fopen("users.txt", "w");
    if (fp == NULL) return;
    for (int i = 0; i < currentUser; i++) {
        fprintf(fp, "%s|%s\n", users[i].username, users[i].password);
    }
    fclose(fp);
}

void loadUsersFromFile() {
    FILE *fp = fopen("users.txt", "r");
    if (fp == NULL) return;
    while (fscanf(fp, "%49[^|]|%49[^\n]\n",
                  users[currentUser].username,
                  users[currentUser].password) == 2) {
        currentUser++;
    }
    fclose(fp);
}

void signUp() {
	printf("\n----^^ Sign Up ^^----\n");

    char username[50], password[50];
    printf("Enter Username (must end with @gmail.com): ");
    scanf("%s", username);
    getchar();

    if (strstr(username, "@gmail.com") == NULL) {
        printf("Invalid email!\n");
        return;
    }

    printf("Enter Password: ");
    scanf("%s", password);
    getchar();

    strcpy(users[currentUser].username, username);
    strcpy(users[currentUser].password, password);
    currentUser++;

    saveUsersToFile();
    printf("Sign Up Successful! ^_^\n");
}

void signIn() {
	printf("\n----^^ Sign In ^^----\n");

    char username[50], password[50];
    printf("Enter Username: ");
    scanf("%s", username);
    getchar();

    printf("Enter Password: ");
    scanf("%s", password);
    getchar();

    if (strcmp(username, "admin") == 0 && strcmp(password, "admin123") == 0) {
        printf("Logging In...\n");
        printf("\n^^ Welcome Admin ^^\n");
        adminDashboard();
        return;
    }

    for (int i = 0; i < currentUser; i++) {
        if (strcmp(users[i].username, username) == 0 &&
            strcmp(users[i].password, password) == 0) {
            printf("Logging In...\n");
            userDashboard(i);
            return;
        }
    }

    printf("Login Failed! j_j\n");
}

void adminDashboard() {
    int choice;
    while (1) {
        printf("\n----- ^^ Admin Dashboard ^^ -----\n");
        printf("1. Check Appointments\n2. Logout\nChoice: ");
        scanf("%d", &choice);
        getchar();

        if (choice == 1) {
            if (currentAppointment == 0) {
                printf("\nNo appointments yet ^^ sadly\n");
            } else {
                printf("\nAppointments:\n");
                for (int i = 0; i < currentAppointment; i++) {
                        if (strcmp(appointments[i].status, "Rejected") == 0) {
                            continue;
                        }
                    printf("%d. %s %s %s ...%s\n",
                           appointments[i].id,
                           appointments[i].firstName,
                           appointments[i].middleName,
                           appointments[i].lastName,
                           appointments[i].status);
                }

                int id;
                printf("\nEnter Appointment ID to view details: ");
                scanf("%d", &id);
                getchar();

                int found = 0;
                for (int i = 0; i < currentAppointment; i++) {
                    if (appointments[i].id == id) {
                        found = 1;
                        printf("\n---^^ Appointment Details ---\n");
                        printf("Full Name: %s %s %s\n",
                               appointments[i].firstName,
                               appointments[i].middleName,
                               appointments[i].lastName);
                        printf("Address: %s\n", appointments[i].address);
                        printf("Contact No.: %s\n", appointments[i].contact);
                        printf("Appointment Details: %s\n", appointments[i].details);
                        printf("------------------------------\n");
                        printf("\n1. Accept\n2. Reject\nChoice: ");
                        int action;
                        scanf("%d", &action);
                        getchar();

                        if (action == 1) {
                            strcpy(appointments[i].status, "Accepted");
                            printf(" Appointment Accepted ^^\n");
                        } else if (action == 2) {
                            strcpy(appointments[i].status, "Rejected");
                            printf(" Appointment Rejected jj\n");
                        } else {
                            printf("Invalid action O_o\n");
                        }
                        saveAppointmentsToFile();
                        break;
                    }
                }

                if (!found) {
                    printf("^^ Appointment ID not found \n");
                }
            }
        } else if (choice == 2) {
            printf("Logging out...\n");
            break;
        } else {
            printf("Invalid choice O_o\n");
        }
    }
}

void userDashboard(int currentUserIndex) {
    int choice;
    while (1) {
        printf("\n----- ^^ User Dashboard ^^ -----\n");
        printf("1. Book Appointment\n2. Check Status\n3. Cancel Appointment\n4. Logout\nChoice: ");
        scanf("%d", &choice);
        getchar();

        if (choice == 1) {
            bookAppointment(currentUserIndex);
        } else if (choice == 2) {
            int hasAppointment = 0;
            for (int i = 0; i < currentAppointment; i++) {
                hasAppointment = 1;
                printf("\n--------------------");
                printf("\nAppointment ID: %d\n", appointments[i].id);
                printf("Status: %s\n", appointments[i].status);
                printf("Full Name: %s %s %s\n",
                       appointments[i].firstName,
                       appointments[i].middleName,
                       appointments[i].lastName);
                printf("Address: %s\n", appointments[i].address);
                printf("Contact No.: %s\n", appointments[i].contact);
                printf("Appointment Details: %s\n", appointments[i].details);
                printf("--------------------\n");

                if (strcmp(appointments[i].status, "Rejected") == 0) {
                    printf("O_O Appointment Form");
                    printf("\n--------------------\n");
                    printf("Fullname: %s %s %s\n", appointments[i].firstName, appointments[i].middleName, appointments[i].lastName);
                    printf("Address: %s\n", appointments[i].address);
                    printf("Contact No.: %s\n", appointments[i].contact);
                    printf("Appointment Details: %s\n", appointments[i].details);
                    printf("--------------------\n");
                    printf("\n--------------------");
                    printf("\nMessage: Rejected");
                    printf("\n--------------------\n");

                    printf("\nDo you want to delete this rejected appointment?\n1.Yes\n2.No\nChoice: ");
                    int delOption;
                    scanf("%d", &delOption);
                    getchar();

                    if (delOption == 1) {
                        for (int j = i; j < currentAppointment - 1; j++) {
                            appointments[j] = appointments[j + 1];
                        }
                        currentAppointment--;
                        saveAppointmentsToFile();
                        printf("^^ Rejected appointment has been deleted successfully.\n");
                        i--;
                    }
                }
            }

            if (!hasAppointment) {
                printf("\nYou have no appointments ^^\n");
            }
        } else if (choice == 3) {
            int id;
            printf("Enter Appointment ID to cancel: ");
            scanf("%d", &id);
            getchar();

            int found = 0;
            for (int i = 0; i < currentAppointment; i++) {
                if (appointments[i].id == id) {
                    found = 1;
                    if (strcmp(appointments[i].status, "Accepted") == 0) {
                        printf("Nonono... Appointment can't be cancelled. It has already been accepted.\n");
                    } else {
                        for (int j = i; j < currentAppointment - 1; j++) {
                            appointments[j] = appointments[j + 1];
                        }
                        currentAppointment--;
                        saveAppointmentsToFile();
                        printf(" ^^ Appointment Cancelled.\n");
                    }
                    break;
                }
            }

            if (!found) {
                printf("^^ Appointment ID not found ^^\n");
            }
        } else {
            break;
        }
    }
}

void bookAppointment(int currentUserIndex) {
    Appointment a;
    a.id = currentAppointment + 1;
    strcpy(a.status, "Pending");

    printf("Lastname: ");
    fgets(a.lastName, sizeof(a.lastName), stdin);
    a.lastName[strcspn(a.lastName, "\n")] = 0;

    printf("firstName: ");
    fgets(a.firstName, sizeof(a.firstName), stdin);
    a.firstName[strcspn(a.firstName, "\n")] = 0;

    printf("Middlename: ");
    fgets(a.middleName, sizeof(a.middleName), stdin);
    a.middleName[strcspn(a.middleName, "\n")] = 0;

    printf("Address: ");
    fgets(a.address, sizeof(a.address), stdin);
    a.address[strcspn(a.address, "\n")] = 0;

    printf("Contact No.: ");
    scanf("%s", a.contact);
    getchar();

    printf("Appointment Details: ");
    fgets(a.details, sizeof(a.details), stdin);
    a.details[strcspn(a.details, "\n")] = 0;

    appointments[currentAppointment++] = a;
    saveAppointmentsToFile();
    printf("\n ^^ Appointment Submitted Successfully!\n");
}