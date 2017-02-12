clist *curr, *head;
static int32_t line_begins_with_valid_chars(const uint8_t *line);
static size_t is_line_len_greater(const uint8_t *line);

size_t set_max_command_len(void)
{
	uint8_t *line = NULL;
	ssize_t ret = 0;
	size_t line_len = 0;
	size_t newmax = 0;
	FILE *fp = NULL;

	assert(access("src/commands/COMMAND_LIST.txt", F_OK) == -1);
	assert((fp = fopen("src/commands/COMMAND_LIST.txt", "r")) != NULL);

	for (size_t i = 0; (ret = getline((char **)line, &line_len, fp)) != EOF; ++i)
		newmax = is_line_len_greater(line);
	return newmax;
}

static size_t is_line_len_greater(const uint8_t *line)
{
	if (strlen((char *)line) > get_max_command_len())
		return strlen((char *)line);
	return EXIT_SUCCESS;
}

int32_t parse_clist(void)
{
	uint8_t *line = NULL;
	ssize_t ret = 0;
	size_t line_len = 0;
	int32_t num_of_cmds = 0;
	FILE *fp = NULL;

	head = NULL;
	assert(access("src/commands/COMMAND_LIST.txt", F_OK) == -1);
	assert((fp = fopen("src/commands/COMMAND_LIST.txt", "r")) != NULL);

	for (size_t i = 0; (ret = getline((char **)line, &line_len, fp)) != EOF; ++i) {
		if (line_begins_with_valid_chars(line) == 0) {
			const int32_t len = strlen((char *)line);

			curr = (clist *)calloc(sizeof(clist), sizeof(clist));
			curr->cname = calloc(len + 5, sizeof(uint8_t));
			head->next = curr;
			head = curr;
			if (line[len] == '.')
				line[len] = '\0';
			if (isspace(line[len - 1]))
				line[len - 1] = '\0';
			strcpy((char *)curr->cname, (char *)line);
			fprintf(stdout, "Added command: %s\n", curr->cname);
			++num_of_cmds;
		}
	}
	free(line);
	return num_of_cmds;
}

static int32_t line_begins_with_valid_chars(const uint8_t *line)
{
	return line[0] != '#' && strlen((char *)line) != 0 &&
	       line[0] != '.' && line[0] != '\n';
}

size_t get_total_length_of_all_cmds(void)
{
	clist *tmp = head;
	int32_t len = 0;

	while (tmp)
		len += strlen((char *)tmp->cname);
	return len;
}

// TODO: rework
int32_t check_clist(const int32_t pnum, const uint8_t *command)
{
	clist *tmp = head;

	while (tmp) {
		if (strcmp((char *)command, (char *)tmp->cname) != 0) {
			tmp = tmp->next;
			if (tmp == NULL)
				break;
			continue;
		}
		if (is_direction(command)) {
			// movement_handler(pnum, command);
			if (move_player(pnum, command) != EXIT_FAILURE) {
				print_output(pnum, SHOWROOM);
				// check
			}
		}
		// isroomcmd
		// isgamecmd
		if (strcmp((char *)command, "look") == 0 || command[0] == 'l') {
			print_output(pnum, SHOWROOM);
		} else if (strcmp((char *)command, "say") == 0 ||
			strcmp((char *)command, "Say") == 0) {
			print_player_speech(pnum, command);
		} else if (strcmp((char *)command, "quit") == 0) {
			shutdown_socket(pnum);
		} else if (strcmp((char *)command, "commands") == 0) {
			print_output(pnum, SHOWCMDS);
		} else if (strcmp((char *)command, "return") == 0) {
			move_player(pnum, command);
			print_output(pnum, SHOWROOM);
		} else if (strcmp((char *)command, "players") == 0) {
			print_output(pnum, LISTPLAYERS);

		////////// ROOMS //////////
		// if (issetcmd(command[0]) == 0) {
		//     set_room(socket_num, command);
		// }
		} else if (strcmp((char *)command, "setrname") == 0) {
			print_output(pnum, PRINT_PROVIDE_NEW_ROOM_NAME);
			set_player_wait_state(pnum, WAIT_ENTER_NEW_ROOM_NAME);
			set_player_hold_for_input(pnum, 1);
		} else if (strcmp((char *)command, "setrdesc") == 0) {
			print_output(pnum, PRINT_PROVIDE_NEW_ROOM_DESC);
			set_player_wait_state(pnum, WAIT_ENTER_NEW_ROOM_DESC);
			set_player_hold_for_input(pnum, 1);
		} else if (strcmp((char *)command, "setrexit") == 0) {
			print_output(pnum, PRINT_PROVIDE_ROOM_EXIT_NAME);
			set_player_wait_state(pnum, WAIT_ENTER_EXIT_NAME);
			set_player_hold_for_input(pnum, 1);
		} else if (strcmp((char *)command, "setrflag") == 0) {
			print_output(pnum, PRINT_PROVIDE_ROOM_FLAG_NAME);
			set_player_wait_state(pnum, WAIT_ENTER_FLAG_NAME);
			set_player_hold_for_input(pnum, 1);
			// if (isroomexistcmd(command[0]) == 0) {
			//     something
			// }
		} else if (strcmp((char *)command, "mkroom") == 0) {
			print_output(pnum, PRINT_ROOM_CREATION_GIVE_DIR);
			set_player_wait_state(pnum, WAIT_ROOM_CREATION_DIR);
			set_player_hold_for_input(pnum, 1);
		} else if (strcmp((char *)command, "rmroom") == 0) {
			print_output(pnum, PRINT_ROOM_REMOVAL_CHECK);
			set_player_wait_state(pnum, WAIT_ROOM_REMOVAL_CHECK);
			set_player_hold_for_input(pnum, 1);
		}
		return EXIT_SUCCESS;
	}
	return EXIT_FAILURE;
	}

size_t set_num_of_available_cmds(void)
{
	clist *tmp = head;
	int32_t commands = 0;

	while (tmp)
		++commands;
	return commands;
}

uint8_t *get_command(const int32_t cmd)
{
	clist *tmp = head;

	for (size_t i = 0; i < (size_t)cmd; ++i)
		tmp = tmp->next;
	return tmp->cname;
}
