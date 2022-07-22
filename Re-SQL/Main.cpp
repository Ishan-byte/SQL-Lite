#include "Source.h"

int main( int argc, char* argv[]) {

	Table* table = new_table();
	InputBuffer* input_buffer = new_input_buffer();

	while (true) {
		print_prompt();
		read_input(input_buffer);


		//meta command check garne ho
		if (input_buffer->buffer[0] == '.') {
			switch (check_meta_command(input_buffer)) {
			case (META_COMMAND_SUCCESS):
				continue;

			case (META_COMMAND_UNRECOGNIZED_COMMAND):
				printf("Unrecognized command '%s' \n", input_buffer->buffer);
				continue;
			}
		}

		Statement statement;

		switch (prepare_statement(input_buffer, &statement)) {
		case PREPARE_SUCCESS:
			break;
		case PREPARE_UNRECOGNIZED_STATEMENT:
			printf("Unrecognized keyword at the start of '%s \n", input_buffer->buffer);
			continue;

		case PREPARE_SYNTAX_ERROR:
			printf("Syntax error. Could not parse statement \n");
			continue;

		}

		execute_statement(&statement);
		printf("Executed. \n");
	}

}



//for printing a prompt to the user
void print_prompt() {
	printf("db > ");
}

void read_input(InputBuffer* input_buffer) {
	ssize_t bytes_read = getline(&(input_buffer->buffer), &(input_buffer->buffer_length), stdin);
	if (bytes_read <= 0) {
		printf("Error handling input \n");
		exit(EXIT_FAILURE);
	}

	input_buffer->input_length = bytes_read - 1;
	input_buffer->buffer[bytes_read - 1] = 0;

}

void close_input_buffer(InputBuffer* input_buffer) {
	free(input_buffer->buffer);
	free(input_buffer);
}


MetaCommandResult check_meta_command(InputBuffer* input_buffer) {
	if (strcmp(input_buffer->buffer, ".exit") == 0) {
		close_input_buffer(input_buffer);
		exit(EXIT_SUCCESS);
	}
	else {
		return META_COMMAND_UNRECOGNIZED_COMMAND;
	}
}

PrepareResult prepare_statement(InputBuffer* input_buffer, Statement* statement) {

	if (strncmp(input_buffer->buffer, "insert", 6) == 0) {
		statement->type = STATEMENT_INSERT;
		int args_assigned = sscanf_s(input_buffer->buffer, "insert %d %s %s", &(statement->row_to_insert.id), statement->row_to_insert.username, statement->row_to_insert.email);
		
		if (args_assigned < 3) {
			return PREPARE_SYNTAX_ERROR;
		}
		
		return PREPARE_SUCCESS;
	}

	
	if (strcmp(input_buffer->buffer, "select") == 0) {
		statement->type = STATEMENT_SELECT;
		return PREPARE_SUCCESS;
	}

	return PREPARE_UNRECOGNIZED_STATEMENT;
}


 void execute_statement(Statement* statement) {
	 switch (statement->type) {
	 case (STATEMENT_INSERT):
		 printf("This is where we would do an insert. \n");
		 break;

	
	 case (STATEMENT_SELECT):
		printf("This is where we would do an select. \n");
		break;
	}
}


 

Table* new_table() {
	Table* table = (Table*)malloc(sizeof(Table));
	table->num_rows = 0;
	for (uint32_t i = 0; i < TABLE_MAX_PAGES; i++) {
		table->pages[i] == NULL;
	}

	return table;
}



void free_Table(Table* table) {
	for (uint32_t i = 0; table->pages[i]; i++) {
		free(table->pages[i]);
	}
	free(table);
}


//void serialize_row(Row* source, void* destination) {
//	memcpy(destination + ID_OFFSET, &(source->id), ID_SIZE);
//}












 





