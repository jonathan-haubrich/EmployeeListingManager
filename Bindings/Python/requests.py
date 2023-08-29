import struct

# enum ELM_REQUEST_TYPES
# {
# 	ELM_REQUEST_ADD_USER = 0,
# 	ELM_REQUEST_REMOVE_USER,
# 	ELM_REQUEST_LIST_USERS,
# 	ELM_REQUEST_GET_USER,
# 	ELM_REQUEST_DISPLAY_USER
# };

# typedef struct _REQUEST_ADD_USER
# {
# 	BYTE bId;
# 	SIZE_T cbListingSize;
# 	BYTE cbFormatter;
# 	CHAR sFormatter[FORMATTER_NAME_MAX_LEN];
# 	BYTE cbFirstName;
# 	CHAR sFirstName[EL_FIRSTNAME_MAX_LEN];
# 	BYTE cbLastName;
# 	CHAR sLastName[EL_LASTNAME_MAX_LEN];
# 	BYTE cbDescription;
# 	CHAR sDescription[1];	// Descriptions can be long so they are variable length
# } 

FORMATTER_NAME_MAX_LEN = 0x10
EL_FIRSTNAME_MAX_LEN = 0x10
EL_LASTNAME_MAX_LEN = 0x10

ELM_REQUEST_ADD_USER = 0
ELM_REQUEST_ADD_USER_FMT = f"!bbLbbbb{FORMATTER_NAME_MAX_LEN}s{EL_FIRSTNAME_MAX_LEN}s{EL_LASTNAME_MAX_LEN}s{{DESCRIPTION_LEN}}s"
def create_request_add_user(user_id, formatter, first_name, last_name, description):
    formatter = formatter.ljust(FORMATTER_NAME_MAX_LEN, '\0').encode()
    first_name = first_name.ljust(EL_FIRSTNAME_MAX_LEN, '\0').encode()
    last_name = last_name.ljust(EL_LASTNAME_MAX_LEN, '\0').encode()
    description = description[:0x100].encode()

    fmt = ELM_REQUEST_ADD_USER_FMT.format(DESCRIPTION_LEN=len(description))
    request = struct.pack(
        fmt,
        ELM_REQUEST_ADD_USER,
        user_id,
        0,
        len(formatter),
        len(first_name),
        len(last_name),
        len(description),
        formatter,
        first_name,
        last_name,
        description
    )

    listing_size = len(request)
    # listing size should not include the command id
    listing_size -= 1
    request = struct.pack(
        fmt,
        ELM_REQUEST_ADD_USER,
        user_id,
        listing_size,
        len(formatter),
        len(first_name),
        len(last_name),
        len(description),
        formatter,
        first_name,
        last_name,
        description
    )

    return request

ELM_REQUEST_REMOVE_USER = 1
ELM_REQUEST_REMOVE_USER_FMT = "!bb"
def create_request_remove_user(user_id):
    return struct.pack(
        ELM_REQUEST_REMOVE_USER_FMT,
        ELM_REQUEST_REMOVE_USER,
        user_id
    )

ELM_REQUEST_LIST_USERS = 2
ELM_REQUEST_LIST_USERS_FMT = "!b"
def create_request_list_users():
    return struct.pack(
        ELM_REQUEST_LIST_USERS_FMT,
        ELM_REQUEST_LIST_USERS,
    )

ELM_REQUEST_GET_USER = 3
ELM_REQUEST_GET_USER_FMT = "!bb"
def create_request_get_user(user_id):
    return struct.pack(
        ELM_REQUEST_GET_USER_FMT,
        ELM_REQUEST_GET_USER,
        user_id
    )

ELM_REQUEST_DISPLAY_USER = 4
ELM_REQUEST_DISPLAY_USER_FMT = "!bb"
def create_request_display_user(user_id):
    return struct.pack(
        ELM_REQUEST_DISPLAY_USER_FMT,
        ELM_REQUEST_DISPLAY_USER,
        user_id
    )

