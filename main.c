#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Type definitions 
typedef enum {
	FALSE, TRUE
} Boolean;
typedef struct Vertex Vertex;
typedef Vertex * VertexPtr;

// Struct Modeling
struct Vertex {
	int data;
	int depth;
	char route[50]; 
	VertexPtr left_child;
	VertexPtr middle_child;
	VertexPtr right_child;
};

// Global Variables
Boolean IS_KEY_FOUND;
Boolean IS_UNIQUE_REMOVE;

// Function prototypes
void read_source_file(VertexPtr *root_ptr,char file_name[]);
void start_user_program(VertexPtr *root_ptr);
void insert_vertex(VertexPtr *root_ptr, int value);
void apply_insertive_traversal(VertexPtr *current_vertex_ptr, VertexPtr new_vertex);
void remove_vertex(VertexPtr *root_ptr, int value);
void apply_removal_traversal(VertexPtr root, int value, VertexPtr *sought_vertex_ptr, VertexPtr *previous_vertex_ptr);
Boolean is_vertex_leave(VertexPtr vertex);
Boolean has_one_child(VertexPtr vertex);
void update_tree(VertexPtr *root_ptr);
void create_temporary_log(VertexPtr root, char temporary_log_file_name[]);
void adjust_tree_after_removal(VertexPtr sought_vertex);
void find_optimum_value(VertexPtr base_left_child, int *optimum_value_ptr);
void record_base_middle_child(VertexPtr base_middle_child);
void find_vertex(VertexPtr root, int value);
void apply_find_traversal(VertexPtr, int value);
void create_ternary_tree_log(VertexPtr root);

int main() {
	VertexPtr root = NULL;
	read_source_file(&root, "input.txt");
	start_user_program(&root);
	puts("Program terminated.");
}

void read_source_file(VertexPtr *root_ptr, char file_name[]) {
	FILE *fptr = fopen(file_name, "r");
	int datum;
	while (fscanf(fptr, "%d\n", &datum) != EOF) {
		insert_vertex(root_ptr, datum);
	}
	fclose(fptr);
}

void start_user_program(VertexPtr *root_ptr) {
	int users_choice, users_value;
	for (;;) {
		puts("***Available Operations***");
		puts("1- Print tree structure");
		puts("2- Insert vertex");
		puts("3- Remove vertex");
		puts("4- Find vertex");
		puts("5- Clear screen");
		puts("6- Quit");
		printf("Please choose the operation: ");
		scanf("%d", &users_choice);
		switch (users_choice) {
			case 1: {
				puts("------------------------------------------------");
				system("more ternary_tree_log.txt");
				puts("------------------------------------------------");
				break;
			}
			case 2: {
				printf("Enter the value to insert: ");
				scanf("%d", &users_value);
				insert_vertex(root_ptr, users_value);
				break;
			}
			case 3: {
				printf("Enter the value to remove: ");
				scanf("%d", &users_value);
				remove_vertex(root_ptr, users_value);
				break;
			}
			case 4: {
				printf("Enter the value to find: ");
				scanf("%d", &users_value);
				find_vertex(*root_ptr, users_value);
				break;
			}
			case 5: {
				system("cls");
				break;
			}
			case 6: return;
			default: puts("Please choose one the digits above!");
		}
	}
}

void insert_vertex(VertexPtr *root_ptr, int value) {
	VertexPtr new_vertex = (VertexPtr)malloc(sizeof(Vertex));
	if (new_vertex != NULL) {
		new_vertex->depth = 0;
		strcpy(new_vertex->route, "");
		new_vertex->data = value;
		new_vertex->left_child = NULL;
		new_vertex->middle_child = NULL;
		new_vertex->right_child = NULL;
		if (*root_ptr == NULL) {
			*root_ptr = new_vertex;
			return;
		}
		VertexPtr current_vertex = *root_ptr;
		apply_insertive_traversal(&current_vertex, new_vertex);
		remove("ternary_tree_log.txt");
		create_ternary_tree_log(*root_ptr);
	} else {
		insert_vertex(root_ptr, value);
	}
}

void apply_insertive_traversal(VertexPtr *current_vertex_ptr, VertexPtr new_vertex) {
	if (*current_vertex_ptr == NULL) {
		*current_vertex_ptr = new_vertex;
	} else {
		new_vertex->depth++;
		if (new_vertex->data < (*current_vertex_ptr)->data) {
			strcat(new_vertex->route, "L");
			apply_insertive_traversal(&(*current_vertex_ptr)->left_child, new_vertex);
		} else if ((*current_vertex_ptr)->data < new_vertex->data && new_vertex->data <= (*current_vertex_ptr)->data * (*current_vertex_ptr)->data) {
			strcat(new_vertex->route, "M");
			apply_insertive_traversal(&(*current_vertex_ptr)->middle_child, new_vertex);
		} else if((*current_vertex_ptr)->data * (*current_vertex_ptr)->data < new_vertex->data) {
			strcat(new_vertex->route, "R");
			apply_insertive_traversal(&(*current_vertex_ptr)->right_child, new_vertex);
		}
	}
}

void remove_vertex(VertexPtr *root_ptr, int value) {
	VertexPtr sought_vertex = NULL, previous_vertex = NULL;
	apply_removal_traversal(*root_ptr, value, &sought_vertex, &previous_vertex);
	if (sought_vertex == NULL) {
		printf("Key is not found.\n");
		return;
	} else if (*root_ptr == sought_vertex && is_vertex_leave(*root_ptr)) {
		*root_ptr = NULL;
		FILE *ternary_tree_fptr = fopen("ternary_tree_log.txt", "w");
		fprintf(ternary_tree_fptr, "%s\n", "Ternary tree is null.");
		fclose(ternary_tree_fptr);
		return;
	} else if (is_vertex_leave(sought_vertex)) {
		if (previous_vertex->left_child == sought_vertex) {
			previous_vertex->left_child = NULL;
		} else if (previous_vertex->middle_child == sought_vertex) {
			previous_vertex->middle_child = NULL;
		} else if (previous_vertex->right_child == sought_vertex) {
			previous_vertex->right_child = NULL;
		}
		free(sought_vertex);
	} else if (has_one_child(sought_vertex)) {
		char bridge_direction = sought_vertex->route[strlen(sought_vertex->route) - 1];
		if (sought_vertex->left_child != NULL) {
			switch (bridge_direction) {
				case 'L': previous_vertex->left_child = sought_vertex->left_child;break;
				case 'M': previous_vertex->middle_child = sought_vertex->left_child;break;
				case 'R': previous_vertex->right_child = sought_vertex->left_child;break;
			}
		} else if (sought_vertex->middle_child != NULL) {
			switch (bridge_direction) {
				case 'L': previous_vertex->left_child = sought_vertex->middle_child;break;
				case 'M': previous_vertex->middle_child = sought_vertex->middle_child;break;
				case 'R': previous_vertex->right_child = sought_vertex->middle_child;break;
			}
		} else if (sought_vertex->right_child != NULL) {
			switch (bridge_direction) {
				case 'L': previous_vertex->left_child = sought_vertex->right_child;break;
				case 'M': previous_vertex->middle_child = sought_vertex->right_child;break;
				case 'R': previous_vertex->right_child = sought_vertex->right_child;break;
			}
		}
		free(sought_vertex);
	} else {
		adjust_tree_after_removal(sought_vertex);
	}
	if (!IS_UNIQUE_REMOVE) {
		update_tree(root_ptr);
	}
}

void apply_removal_traversal(VertexPtr root, int value, VertexPtr *sought_vertex_ptr, VertexPtr *previous_vertex_ptr) {
	if (root == NULL) {
		return;
	} else if (root->left_child != NULL && root->left_child->data == value) {
		*previous_vertex_ptr = root;
	} else if (root->middle_child != NULL && root->middle_child->data == value) {
		*previous_vertex_ptr = root;
	} else if (root->right_child != NULL && root->right_child->data == value) {
		*previous_vertex_ptr = root;
	} else if (root->data == value) {
		IS_KEY_FOUND = TRUE;
		*sought_vertex_ptr = root;
	}
	apply_removal_traversal(root->left_child, value, sought_vertex_ptr, previous_vertex_ptr);
	apply_removal_traversal(root->middle_child, value, sought_vertex_ptr, previous_vertex_ptr);
	apply_removal_traversal(root->right_child, value, sought_vertex_ptr, previous_vertex_ptr);
}

Boolean is_vertex_leave(VertexPtr vertex) {
	if (vertex->left_child == NULL && vertex->middle_child == NULL && vertex->right_child == NULL) {
		return TRUE;
	} else {
		return FALSE;
	}
}

Boolean has_one_child(VertexPtr vertex) {
	if (vertex->left_child != NULL && vertex->middle_child == NULL && vertex->right_child == NULL) {
		return TRUE;
	} else if (vertex->left_child == NULL && vertex->middle_child != NULL && vertex->right_child == NULL) {
		return TRUE;
	} else if (vertex->left_child == NULL && vertex->middle_child == NULL && vertex->right_child != NULL) {
		return TRUE;
	} else {
		return FALSE;
	}
}

void update_tree(VertexPtr *root_ptr) {
	create_temporary_log(*root_ptr, "temporary_log.txt");
	*root_ptr = NULL;
	read_source_file(root_ptr, "temporary_log.txt");
	remove("temporary_log.txt");
}

void create_temporary_log(VertexPtr root, char temporary_log_file_name[]) {
	if (root == NULL) {
		return;
	}
	FILE *temporary_log_file = fopen("temporary_log.txt", "a");
	fprintf(temporary_log_file, "%d\n", root->data);
	fclose(temporary_log_file);
	create_temporary_log(root->left_child, temporary_log_file_name);
	create_temporary_log(root->middle_child, temporary_log_file_name);
	create_temporary_log(root->right_child, temporary_log_file_name);
}

void adjust_tree_after_removal(VertexPtr sought_vertex) {
	int optimum_value = sought_vertex->left_child->data, value;
	find_optimum_value(sought_vertex->left_child, &optimum_value);
	sought_vertex->data = optimum_value;
	IS_UNIQUE_REMOVE = TRUE;
	remove_vertex(&sought_vertex, optimum_value);
	IS_UNIQUE_REMOVE = FALSE;
	record_base_middle_child(sought_vertex->middle_child);
	sought_vertex->middle_child = NULL;
	free(sought_vertex->middle_child);
	FILE * base_middle_data_fptr = fopen("base_middle_child_log.txt", "r");
	while (fscanf(base_middle_data_fptr, "%d\n", &value) != EOF) {
		insert_vertex(&sought_vertex, value);
	}
	fclose(base_middle_data_fptr);
	remove("base_middle_child_log.txt");
}

void find_optimum_value(VertexPtr base_left_child, int *optimum_value_ptr) {
	if (base_left_child == NULL) {
		return;
	}
	*optimum_value_ptr = (base_left_child->data > *optimum_value_ptr) ? base_left_child->data : *optimum_value_ptr;
	find_optimum_value(base_left_child->left_child, optimum_value_ptr);
	find_optimum_value(base_left_child->middle_child, optimum_value_ptr);
	find_optimum_value(base_left_child->right_child, optimum_value_ptr);
}

void record_base_middle_child(VertexPtr base_middle_child) {
	if (base_middle_child == NULL) {
		return;
	}
	FILE *base_middle_data_fptr = fopen("base_middle_child_log.txt", "a");
	fprintf(base_middle_data_fptr, "%d\n", base_middle_child->data);
	fclose(base_middle_data_fptr);
	record_base_middle_child(base_middle_child->left_child);
	record_base_middle_child(base_middle_child->middle_child);
	record_base_middle_child(base_middle_child->right_child);
}

void find_vertex(VertexPtr root, int value) {
	IS_KEY_FOUND = FALSE;
	apply_find_traversal(root, value);
	if (!IS_KEY_FOUND) {
		printf("Key is not found.\n");
	}
}

void apply_find_traversal(VertexPtr root, int value) {
	if (root == NULL) {
		return;
	} else if (root->data == value) {
		IS_KEY_FOUND = TRUE;
		char first_format[5], second_format[5], third_format[50];
		sprintf(first_format, "[%d]", root->data);
		sprintf(second_format, "[%d]", root->depth);
		sprintf(third_format, "[%s]", root->route);
		printf("Value: %-7s Depth: %-7s Route: %-7s\n", first_format, second_format, third_format);
	}
	apply_find_traversal(root->left_child, value);
	apply_find_traversal(root->middle_child, value);
	apply_find_traversal(root->right_child, value);
}

void create_ternary_tree_log(VertexPtr root) {
	if (root == NULL) {
		return;
	}
	FILE *fptr = fopen("ternary_tree_log.txt", "a");
	char first_format[10], second_format[10];
	sprintf(first_format, "[%d]", root->data);
	sprintf(second_format, "[%d]", root->depth);
	fprintf(fptr, "Value: %-7s Depth: %-7s Route: [%s]\n", first_format, second_format, root->route);
	fclose(fptr);
	create_ternary_tree_log(root->left_child);
	create_ternary_tree_log(root->middle_child);
	create_ternary_tree_log(root->right_child);
}