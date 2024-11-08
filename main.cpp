#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct student
{
    char *name;
    int id;
    char *address;
    struct student *next;
};

struct student* create_student(char *name, int id, char *address)
{
    struct student *new_student = (struct student*)malloc(sizeof(struct student));
    if (!new_student)
    {
        printf("Memory allocation failed\n");
        return NULL;
    }
    new_student->name = (char*)malloc(strlen(name) + 1);
    new_student->address = (char*)malloc(strlen(address) + 1);
    strcpy(new_student->name, name);
    new_student->id = id;
    strcpy(new_student->address, address);
    new_student->next = NULL;
    return new_student;
}

void display_students(struct student *head)
{
    if (!head)
    {
        printf("No students to display.\n");
        return;
    }
    struct student *current = head;
    while (current)
    {
        printf("Name: %s, ID: %d, Address: %s\n", current->name, current->id, current->address);
        current = current->next;
    }
}

void cut_and_swap(struct student **head, char *name1, char *name2)
{
    struct student *prev1 = NULL, *prev2 = NULL, *node1 = *head, *node2 = *head;

    while (node1 && strcmp(node1->name, name1) != 0)
    {
        prev1 = node1;
        node1 = node1->next;
    }
    while (node2 && strcmp(node2->name, name2) != 0)
    {
        prev2 = node2;
        node2 = node2->next;
    }

    if (!node1 || !node2)
    {
        printf("One or both students not found.\n");
        return;
    }
    if (node1 == node2) return;

    if (prev1) prev1->next = node2;
    else *head = node2;

    if (prev2) prev2->next = node1;
    else *head = node1;

    struct student *temp = node1->next;
    node1->next = node2->next;
    node2->next = temp;
}

void insert_at_index(struct student **head, int index, char *name, int id, char *address)
{
    struct student *new_student = create_student(name, id, address);
    if (index == 0)
    {
        new_student->next = *head;
        *head = new_student;
    } else
    {
        struct student *current = *head;
        for (int i = 0; i < index - 1 && current; i++)
        {
            current = current->next;
        }
        if (!current)
        {
            printf("Index out of bounds.\n");
            free(new_student->name);
            free(new_student->address);
            free(new_student);
            return;
        }
        new_student->next = current->next;
        current->next = new_student;
    }
}

void delete_at_index(struct student **head, int index)
{
    if (*head == NULL)
    {
        printf("List is empty.\n");
        return;
    }

    struct student *temp = *head;
    if (index == 0)
    {
        *head = temp->next;
        free(temp->name);
        free(temp->address);
        free(temp);
        return;
    }
    for (int i = 0; temp && i < index - 1; i++)
    {
        temp = temp->next;
    }

    if (!temp || !temp->next)
    {
        printf("Index out of bounds.\n");
        return;
    }

    struct student *next_ptr = temp->next->next;
    free(temp->next->name);
    free(temp->next->address);
    free(temp->next);
    temp->next = next_ptr;
}

void save_to_file(struct student *head, const char *filename)
{
    FILE *file = fopen(filename, "a");
    if (!file)
    {
        printf("Could not open file for writing.\n");
        return;
    }
    struct student *current = head;
    while (current)
    {
        fprintf(file, "%d %s %s\n", current->id, current->name, current->address);
        current = current->next;
    }
    fclose(file);
    printf("Data saved to file successfully.\n");
}

void display_file_data(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        printf("Could not open file for reading.\n");
        return;
    }

    int id;
    char name[50], address[50];
    printf("Data in file:\n");
    while (fscanf(file, "%d %s %s", &id, name, address) != EOF)
    {
        printf("ID: %d, Name: %s, Address: %s\n", id, name, address);
    }
    fclose(file);
}

void sync_from_file(struct student **head, const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        printf("Could not open file for reading.\n");
        return;
    }

    struct student *new_head = NULL, *tail = NULL;
    int id;
    char name[50], address[50];

    while (fscanf(file, "%d %s %s", &id, name, address) != EOF)
    {
        struct student *new_student = create_student(name, id, address);
        if (!new_head)
        {
            new_head = tail = new_student;
        } else
        {
            tail->next = new_student;
            tail = new_student;
        }
    }

    fclose(file);

    struct student *current = *head;
    while (current)
    {
        struct student *temp = current;
        current = current->next;
        free(temp->name);
        free(temp->address);
        free(temp);
    }

    *head = new_head;
    printf("Data synchronized from file successfully.\n");
}

int main() {
    struct student *head = NULL;
    int choice;
    const char *filename = "likhi.txt";

    while (1)
    {
        printf("\nOptions:\n1. Create a chain\n2. Cut and reconnect students using swap\n3. Display students\n");
        printf("4. Insert node by index\n5. Delete node by index\n6. Save to file\n7. Display file data\n");
        printf("8. Sync from file\n9. Exit\nEnter your choice: ");
        scanf("%d", &choice);

        if (choice == 1)
        {
            int n;
            printf("Number of students to add: ");
            scanf("%d", &n);
            for (int i = 0; i < n; i++)
            {
                char name[50], address[50];
                int id;

                printf("Enter student's ID: ");
                scanf("%d", &id);
                printf("Enter student's name: ");
                scanf("%s", name);
                printf("Enter student's address: ");
                scanf("%s", address);

                struct student *new_student = create_student(name, id, address);

                if (!head)
                {
                    head = new_student;
                }
                else
                {
                    struct student *current = head;
                    while (current->next)
                    {
                        current = current->next;
                    }
                    current->next = new_student;
                }
                printf("Student %s added successfully.\n", name);
            }
        }
        else if (choice == 2)
        {
            char name1[50], name2[50];
            printf("Enter the first student's name to cut: ");
            scanf("%s", name1);
            printf("Enter the second student's name to link: ");
            scanf("%s", name2);
            cut_and_swap(&head, name1, name2);
        }
        else if (choice == 3)
        {
            display_students(head);
        }
        else if (choice == 4)
        {
            char name[50], address[50];
            int id, index;
            printf("Enter index to insert at: ");
            scanf("%d", &index);
            printf("Enter student's ID: ");
            scanf("%d", &id);
            printf("Enter student's name: ");
            scanf("%s", name);
            printf("Enter student's address: ");
            scanf("%s", address);
            insert_at_index(&head, index, name, id, address);
        }
        else if (choice == 5)
        {
            int index;
            printf("Enter index to delete: ");
            scanf("%d", &index);
            delete_at_index(&head, index);
        }
        else if (choice == 6)
        {
            save_to_file(head, filename);
        }
        else if (choice == 7)
        {
            display_file_data(filename);
        }
        else if (choice == 8)
        {
            sync_from_file(&head, filename);
        }
        else if (choice == 9)
        {
            break;
        }
        else
        {
            printf("Invalid choice.\n");
        }
    }

    struct student *current = head;
    while (current)
    {
        struct student *temp = current;
        current = current->next;
        free(temp->name);
        free(temp->address);
        free(temp);
    }

    return 0;
}
