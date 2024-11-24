#include "raylib.h"
#include <stdlib.h>
#include <math.h>

// Define the Node structure
typedef struct Node {
    float x, y;             // Current position of the circle
    float angle;            // Rotation angle
    float angularSpeed;     // Speed of rotation
    float radius;           // Circle radius
    Color color;            // Circle color
    float lineLength;       // Line length connecting to the parent
    struct Node* next;      // Pointer to the next node
} Node;

// Create a new node with random properties
Node* CreateNode(float parentX, float parentY) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->angle = 0.0f;
    newNode->angularSpeed = (rand() % 200 - 100) / 100.0f; // Random speed
    newNode->radius = rand() % 20 + 10;                    // Random radius
    newNode->lineLength = rand() % 100 + 50;               // Random line length
    newNode->color = (Color){ rand() % 256, rand() % 256, rand() % 256, 255 };
    newNode->next = NULL;
    newNode->x = parentX;
    newNode->y = parentY;
    return newNode;
}

// Add a new node at the tail of the linked list
void AddNode(Node* root) {
    Node* current = root;
    while (current->next != NULL) {
        current = current->next;
    }
    Node* newNode = CreateNode(current->x, current->y);
    current->next = newNode;
}

// Remove the tail node of the linked list
void RemoveNode(Node* root) {
    if (root->next == NULL) return; // Root node cannot be removed

    Node* current = root;
    while (current->next->next != NULL) {
        current = current->next;
    }
    free(current->next);
    current->next = NULL;
}

// Update positions and angles of all nodes
void UpdateNodes(Node* root, float deltaTime) {
    Node* current = root->next;
    float parentX = root->x, parentY = root->y;

    while (current != NULL) {
        current->angle += current->angularSpeed * deltaTime;
        current->x = parentX + cosf(current->angle) * current->lineLength;
        current->y = parentY + sinf(current->angle) * current->lineLength;

        parentX = current->x;
        parentY = current->y;

        current = current->next;
    }
}

// Draw all nodes and connecting lines
void DrawNodes(Node* root) {
    Node* current = root;
    Vector2 parentPosition = { root->x, root->y };

    while (current != NULL) {
        if (current != root) {
            // Draw line connecting to the parent
            DrawLineEx(parentPosition, (Vector2) { current->x, current->y }, 2.0f, BLACK);
        }

        // Draw the circle
        DrawCircle(current->x, current->y, current->radius, current->color);

        parentPosition = (Vector2){ current->x, current->y };
        current = current->next;
    }
}

// Free all nodes in the linked list
void FreeNodes(Node* root) {
    Node* current = root;
    while (current != NULL) {
        Node* temp = current;
        current = current->next;
        free(temp);
    }
}

// Main function
int main() {
    // Initialization
    const int screenWidth = 800;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "Multi-Arm Pendulum with Linked List");
    SetTargetFPS(60);

    // Create the root node
    Node* root = (Node*)malloc(sizeof(Node));
    root->x = screenWidth / 2.0f;
    root->y = screenHeight / 2.0f;
    root->angle = 0.0f;
    root->angularSpeed = 0.0f;
    root->radius = 20.0f;
    root->color = RED;
    root->lineLength = 0.0f;
    root->next = NULL;

    // Main game loop
    while (!WindowShouldClose()) {
        // Update
        float deltaTime = GetFrameTime();
        UpdateNodes(root, deltaTime);

        // Handle input
        if (IsKeyPressed(KEY_A)) AddNode(root);  // Add a new node
        if (IsKeyPressed(KEY_R)) RemoveNode(root); // Remove the last node

        // Draw
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawNodes(root);
        EndDrawing();
    }

    // Cleanup
    FreeNodes(root);
    CloseWindow();

    return 0;
}
