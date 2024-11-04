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
    for (struct student *current = head; current; current = current->next)
    {
        printf("Name: %s, ID: %d, Address: %s\n", current->name, current->id, current->address);
    }
}


void cut_and_swap(struct student **head, char *name1, char *name2)
{
    struct student *prev1 = NULL, *prev2 = NULL, *node1 = *head, *node2 = *head;


    while (node1 && strcmp(node1->name, name1) != 0)
    {
        prev1 = node1;//node1 is current node
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


    if (node1 == node2)
    {

        return;
    }


    if (prev1)
    {
        prev1->next = node2;
    }
    else
    {
        *head = node2;
    }
    if (prev2)
    {
        prev2->next = node1;
    }
    else
    {
        *head = node1;
    }
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
        return;
    }

    struct student *current = *head;
    for (int i = 0; i < index - 1 && current != NULL; i++)
    {
        current = current->next;
    }
    if (current == NULL)
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
    for (int i = 0; temp != NULL && i < index - 1; i++)
    {
        temp = temp->next;
    }

    if (temp == NULL || temp->next == NULL)
    {
        printf("Index out of bounds.\n");
        return;
    }

    struct student *next = temp->next->next;
    free(temp->next->name);
    free(temp->next->address);
    free(temp->next);
    temp->next = next;
}

void display_sorted_by_name(struct student *head)
{
    if (!head)
    {
        printf("No students to display.\n");
        return;
    }

    struct student *sorted = NULL;
    struct student *current = head;

    while (current != NULL)
    {
        struct student *next = current->next;
        if (sorted == NULL || strcmp(sorted->name, current->name) >= 0)
        {
            current->next = sorted;
            sorted = current;
        }
        else
        {
            struct student *s = sorted;
            while (s->next != NULL && strcmp(s->next->name, current->name) < 0)
            {
                s = s->next;
            }
            current->next = s->next;
            s->next = current;
        }
        current = next;
    }
    display_students(sorted);

    current = sorted;
    while (current)
    {
        struct student *temp = current;
        current = current->next;
        free(temp->name);
        free(temp->address);
        free(temp);
    }
}

void display_sorted_by_id(struct student *head)
{
    if (!head)
    {
        printf("No students to display.\n");
        return;
    }

    struct student *sorted = NULL;
    struct student *current = head;

    while (current != NULL)
    {
        struct student *next = current->next;
        if (sorted == NULL || sorted->id > current->id)
        {
            current->next = sorted;
            sorted = current;
        }
        else
        {
            struct student *s = sorted;
            while (s->next != NULL && s->next->id < current->id)
            {
                s = s->next;
            }
            current->next = s->next;
            s->next = current;
        }
        current = next;
    }

    display_students(sorted);


    current = sorted;
    while (current)
    {
        struct student *temp = current;
        current = current->next;
        free(temp->name);
        free(temp->address);
        free(temp);
    }
}

int main()
{
    struct student *head = NULL;
    int choice;

    while (1)
    {
        printf("\nOptions:\n1. Create a chain.\n2. Cut and reconnect students using swap\n3. Display students\n4. Insert node by index\n5. Delete node by index\n6. Display sorted by name\n7. Display sorted by ID\n8. Exit\nEnter your choice: ");
        scanf("%d", &choice);

        if (choice == 1)
        {
            int n;
            printf("Number of students required: ");
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

                if (head == NULL)
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
            printf("Enter the name to cut: ");
            scanf("%s", name1);
            printf("Enter the name to link: ");
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
            display_sorted_by_name(head);
        }
        else if (choice == 7) {
            display_sorted_by_id(head);
        }
        else if (choice == 8)
        {
            break;
        }
        else
        {
            printf("Invalid choice, please try again.\n");
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

