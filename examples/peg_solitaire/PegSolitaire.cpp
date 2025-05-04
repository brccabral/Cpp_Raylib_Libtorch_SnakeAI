#include <cstdio>
#include "PegSolitaire.h"


PegSolitaire::Board::~Board()
{
    delete[] pegs;
}


size_t PegSolitaire::index_from_2d(const int col, const int row) const
{
    return (row * board.cols) + col;
}

void PegSolitaire::reset_board() const
{
    if (board.pegs == NULL)
    {
        return;
    }
    const char *pegs;
    switch (board.type)
    {
        case BOARD_TYPE_ENGLISH:
        {
            pegs = "..XXX.."
                   "..XXX.."
                   "XXXXXXX"
                   "XXXXXXX"
                   "XXXXXXX"
                   "..XXX.."
                   "..XXX..";
            break;
        }
        case BOARD_TYPE_EUROPEAN:
        {
            pegs = "..XXX.."
                   ".XXXXX."
                   "XXXXXXX"
                   "XXXXXXX"
                   "XXXXXXX"
                   ".XXXXX."
                   "..XXX..";
            break;
        }
        case BOARD_TYPE_GERMAN:
        {
            pegs = "...XXX..."
                   "...XXX..."
                   "...XXX..."
                   "XXXXXXXXX"
                   "XXXXXXXXX"
                   "XXXXXXXXX"
                   "...XXX..."
                   "...XXX..."
                   "...XXX...";
            break;
        }
        case BOARD_TYPE_ASYMMETRICAL:
        {
            pegs = "..XXX..."
                   "..XXX..."
                   "..XXX..."
                   "XXXXXXXX"
                   "XXXXXXXX"
                   "XXXXXXXX"
                   "..XXX..."
                   "..XXX...";
            break;
        }
        case BOARD_TYPE_DIAMOND:
        {
            pegs = "....X...."
                   "...XXX..."
                   "..XXXXX.."
                   ".XXXXXXX."
                   "XXXXXXXXX"
                   ".XXXXXXX."
                   "..XXXXX.."
                   "...XXX..."
                   "....X....";
            break;
        }
        case BOARD_TYPE_TRIANGULAR:
        {
            pegs = "....X...."
                   "...X.X..."
                   "..X.X.X.."
                   ".X.X.X.X."
                   "X.X.X.X.X";
            break;
        }
        default:
        {
            (void) fprintf(stderr, "ERROR: Invalid board type\n");
            return;
        }
    }

    for (size_t c = 0; c < board.cols; ++c)
    {
        for (size_t r = 0; r < board.rows; ++r)
        {
            const size_t index = index_from_2d(c, r);
            if (pegs[index] == 'X')
            {
                board.pegs[index] = PEG_STATUS_FILLED;
            }
            else
            {
                board.pegs[index] = PEG_STATUS_INVALID;
            }
        }
    }

    const size_t init_index = index_from_2d(board.init_col, board.init_row);
    board.pegs[init_index] = PEG_STATUS_EMPTY;

    if (board.type == BOARD_TYPE_EUROPEAN)
    {
        const size_t init_traced =
                index_from_2d(board.cols - board.init_col - 1, board.rows - board.init_row - 1);
        board.pegs[init_traced] = PEG_STATUS_TRACED;
    }
}

void PegSolitaire::create_board(const board_type type)
{
    board.type = type;
    if (board.pegs != NULL)
    {
        delete[] board.pegs;
    }
    switch (type)
    {
        case BOARD_TYPE_ENGLISH:
        {
            board.cols = 7;
            board.rows = 7;
            board.init_col = 3;
            board.init_row = 3;
            break;
        }
        case BOARD_TYPE_EUROPEAN:
        {
            board.cols = 7;
            board.rows = 7;
            board.init_col = 3;
            board.init_row = 2;
            break;
        }
        case BOARD_TYPE_GERMAN:
        {
            board.cols = 9;
            board.rows = 9;
            board.init_col = 4;
            board.init_row = 4;
            break;
        }
        case BOARD_TYPE_ASYMMETRICAL:
        {
            board.cols = 8;
            board.rows = 8;
            board.init_col = 3;
            board.init_row = 4;
            break;
        }
        case BOARD_TYPE_DIAMOND:
        {
            board.cols = 9;
            board.rows = 9;
            board.init_col = 4;
            board.init_row = 4;
            break;
        }
        case BOARD_TYPE_TRIANGULAR:
        {
            board.cols = 9;
            board.rows = 5;
            board.init_col = 4;
            board.init_row = 0;
            break;
        }
        default:
        {
            printf("Invalid board type\n");
        }
    }
    board.pegs = new peg_status[board.cols * board.rows]{};
    board.size = board.cols * board.rows;
}

int PegSolitaire::can_move_right(const int index_x, const int index_y) const
{
    if (index_x + 2 >= (int) board.cols)
    {
        return 0;
    }

    const int next1 = (index_y * board.cols) + index_x + 1;
    const int next2 = (index_y * board.cols) + index_x + 2;

    if ((board.pegs[next1] == PEG_STATUS_FILLED || board.pegs[next1] == PEG_STATUS_TRACED) &&
        board.pegs[next2] == PEG_STATUS_EMPTY)
    {
        return 1;
    }
    return 0;
}

int PegSolitaire::can_move_left(const int index_x, const int index_y) const
{
    if (index_x - 2 < 0)
    {
        return 0;
    }

    const int next1 = (index_y * board.cols) + index_x - 1;
    const int next2 = (index_y * board.cols) + index_x - 2;

    if ((board.pegs[next1] == PEG_STATUS_FILLED || board.pegs[next1] == PEG_STATUS_TRACED) &&
        board.pegs[next2] == PEG_STATUS_EMPTY)
    {
        return 1;
    }
    return 0;
}

int PegSolitaire::can_move_up(const int index_x, const int index_y) const
{
    if (index_y - 2 < 0)
    {
        return 0;
    }

    const int next1 = ((index_y - 1) * board.cols) + index_x;
    const int next2 = ((index_y - 2) * board.cols) + index_x;

    if ((board.pegs[next1] == PEG_STATUS_FILLED || board.pegs[next1] == PEG_STATUS_TRACED) &&
        board.pegs[next2] == PEG_STATUS_EMPTY)
    {
        return 1;
    }
    return 0;
}

int PegSolitaire::can_move_down(const int index_x, const int index_y) const
{
    if (index_y + 2 >= (int) board.rows)
    {
        return 0;
    }

    const int next1 = ((index_y + 1) * board.cols) + index_x;
    const int next2 = ((index_y + 2) * board.cols) + index_x;

    if ((board.pegs[next1] == PEG_STATUS_FILLED || board.pegs[next1] == PEG_STATUS_TRACED) &&
        board.pegs[next2] == PEG_STATUS_EMPTY)
    {
        return 1;
    }
    return 0;
}

int PegSolitaire::count_peg_moves(const int index) const
{
    if (board.pegs == NULL)
    {
        return 0;
    }

    if (board.pegs[index] == PEG_STATUS_INVALID || board.pegs[index] == PEG_STATUS_EMPTY)
    {
        return 0;
    }

    int result = 0;
    size_t index_x;
    size_t index_y;
    xy_from_index(index, index_x, index_y);

    result += can_move_right(index_x, index_y);
    result += can_move_left(index_x, index_y);
    result += can_move_up(index_x, index_y);
    result += can_move_down(index_x, index_y);

    return result;
}

void PegSolitaire::set_status(const game_status status_)
{
    status = status_;
}

int PegSolitaire::count_movables() const
{
    int count_moveable = 0;
    int count_filled = 0;
    int count_traced = 0;
    for (size_t i = 0; i < board.size; ++i)
    {
        if (board.pegs[i] == PEG_STATUS_FILLED)
        {
            ++count_filled;
        }
        if (board.pegs[i] == PEG_STATUS_TRACED)
        {
            ++count_traced;
        }
        count_moveable += count_peg_moves(i);
    }
    return count_moveable;
}

int PegSolitaire::find_traced() const
{
    for (size_t i = 0; i < board.size; ++i)
    {
        if (board.pegs[i] == PEG_STATUS_TRACED)
        {
            return i;
        }
    }
    return -1;
}

int PegSolitaire::check_game_status()
{
    // too many wrong selections
    if (selections > 5 * board.size)
    {
        set_status(GAME_OVER_LOST);
        return -1;
    }

    const int count_moveable = count_movables();
    const int score = get_score();

    if (count_moveable == 0 && score > 1)
    {
        set_status(GAME_OVER_LOST);
        return -1;
    }

    if (board.type == BOARD_TYPE_EUROPEAN)
    {
        const int index_traced = find_traced();
        if (index_traced == -1)
        {
            set_status(GAME_OVER_LOST);
            return -1;
        }
        size_t traced_x;
        size_t traced_y;
        xy_from_index(index_traced, traced_x, traced_y);
        if (count_moveable == 0 && traced_x == board.init_col && traced_y == board.init_row)
        {
            set_status(GAME_OVER_WON);
            return 1;
        }
    }
    else if (count_moveable == 0 && score == 1)
    {
        set_status(GAME_OVER_WON);
        return 1;
    }

    set_status(GAME_PLAYING);
    return 0;
}

int PegSolitaire::move_peg()
{
    // current
    size_t cursor_x;
    size_t cursor_y;
    // previous
    xy_from_index(cursor, cursor_x, cursor_y);
    size_t selected_x;
    size_t selected_y;
    xy_from_index(selected, selected_x, selected_y);

    int jumped = -1;
    // right
    if (cursor_x == selected_x + 2 && cursor_y == selected_y)
    {
        jumped = index_from_2d(selected_x + 1, selected_y);
    }
    // left
    if (cursor_x + 2 == selected_x && cursor_y == selected_y)
    {
        jumped = index_from_2d(cursor_x + 1, selected_y);
    }
    // down
    if (cursor_x == selected_x && cursor_y == selected_y + 2)
    {
        jumped = index_from_2d(selected_x, selected_y + 1);
    }
    // up
    if (cursor_x == selected_x && cursor_y + 2 == selected_y)
    {
        jumped = index_from_2d(selected_x, cursor_y + 1);
    }
    if (jumped == -1)
    {
        return -1;
    }
    if (board.pegs[jumped] == PEG_STATUS_INVALID || board.pegs[jumped] == PEG_STATUS_EMPTY)
    {
        return -1;
    }
    const peg_status selected_status = board.pegs[selected];
    board.pegs[cursor] = selected_status;
    board.pegs[selected] = PEG_STATUS_EMPTY;
    board.pegs[jumped] = PEG_STATUS_EMPTY;
    selections = 0;
    selected = -1;
    return 2;
}

int PegSolitaire::update_selected()
{
    ++selections;
    // allow select the same, but no points
    if (selected == (int) cursor)
    {
        selections = 0;
        return 0;
    }
    size_t cursor_x;
    size_t cursor_y;
    xy_from_index(cursor, cursor_x, cursor_y);
    // allow select a valid peg, but no points
    if (can_move(cursor_x, cursor_y))
    {
        selected = cursor;
        selections = 0;
        return 0;
    }

    if (selected != -1)
    {
        if (can_receive(cursor_x, cursor_y))
        {
            return move_peg();
        }
    }

    selected = -1;
    return -1;
}

PegSolitaire::game_status PegSolitaire::get_status() const
{
    return status;
}

void PegSolitaire::reset()
{
    reset_board();
    cursor = index_from_2d(board.init_col, board.init_row);
    selected = -1;
    selections = 0;
    status = GAME_PLAYING;
}

void PegSolitaire::change_board(const board_type type)
{
    create_board(type);

    reset();
}

void PegSolitaire::init_game(const board_type type)
{
    create_board(type);

    reset_board();
    cursor = index_from_2d(board.init_col, board.init_row);
    selected = -1;
    status = GAME_SELECTION;
}

PegSolitaire::peg_status *PegSolitaire::get_pegs() const
{
    return board.pegs;
}

size_t PegSolitaire::get_cols() const
{
    return board.cols;
}

size_t PegSolitaire::get_rows() const
{
    return board.rows;
}

size_t PegSolitaire::get_cursor() const
{
    return cursor;
}

int PegSolitaire::get_selected() const
{
    return selected;
}

int PegSolitaire::get_score() const
{
    int result = 0;
    for (size_t i = 0; i < board.size; ++i)
    {
        if (board.pegs[i] == PEG_STATUS_FILLED || board.pegs[i] == PEG_STATUS_TRACED)
        {
            ++result;
        }
    }
    return result;
}

size_t PegSolitaire::get_state_size() const
{
    return board.size + 2; // + is_selected, selected_same_cursor
}

int PegSolitaire::can_move(int index_x, int index_y) const
{
    const int index = (index_y * board.cols) + index_x;
    if (board.pegs[index] == PEG_STATUS_INVALID || board.pegs[index] == PEG_STATUS_EMPTY)
    {
        return 0;
    }

    return can_move_right(index_x, index_y) || can_move_left(index_x, index_y) ||
           can_move_down(index_x, index_y) || can_move_up(index_x, index_y);
}

std::vector<float> PegSolitaire::get_state() const
{
    std::vector<float> result(get_state_size(), 0.0);

    if (selected >= 0)
    {
        for (size_t r = 0; r < board.rows; ++r)
        {
            for (size_t c = 0; c < board.cols; ++c)
            {
                result[index_from_2d(c, r)] = can_receive(c, r);
            }
        }
    }
    else
    {
        for (size_t r = 0; r < board.rows; ++r)
        {
            for (size_t c = 0; c < board.cols; ++c)
            {
                result[index_from_2d(c, r)] = can_move(c, r);
            }
        }
    }
    result[board.size] = selected == -1;
    result[board.size + 1] = selected == (int) cursor;
    return result;
}

StepResult PegSolitaire::get_step(int index)
{
    StepResult result;

    cursor = index;
    result.reward = update_selected();

    const int status = check_game_status();
    if (status != 0)
    {
        result.game_over = true;
        if (count_movables() == 0)
        {
            result.reward = status * 10;
        }
    }

    return result;
}

size_t PegSolitaire::get_action_count() const
{
    return board.size;
}

void PegSolitaire::xy_from_index(size_t index, size_t &col, size_t &row) const
{
    col = index % board.cols;
    row = index / board.cols;
}

int PegSolitaire::can_receive(int index_x, int index_y) const
{
    const auto index = index_from_2d(index_x, index_y);
    const auto status = board.pegs[index];
    if (status == PEG_STATUS_INVALID)
    {
        return 0;
    }
    if (status != PEG_STATUS_EMPTY)
    {
        return 0;
    }
    if (selected == -1)
    {
        return 0;
    }

    return can_receive_right(index_x, index_y) || can_receive_left(index_x, index_y) ||
           can_receive_down(index_x, index_y) || can_receive_up(index_x, index_y);
}

int PegSolitaire::can_receive_right(int index_x, int index_y) const
{
    const int right2_x = index_x + 2;
    if (right2_x >= (int) board.cols)
    {
        return 0;
    }
    const int right2_y = index_y;
    const auto right2_index = index_from_2d(right2_x, right2_y);

    if (right2_index != selected)
    {
        return 0;
    }

    const auto status2 = board.pegs[right2_index];

    const int right1_x = index_x + 1;
    const int right1_y = index_y;
    const auto right1_index = index_from_2d(right1_x, right1_y);
    const auto status1 = board.pegs[right1_index];

    if ((status1 == PEG_STATUS_FILLED || status1 == PEG_STATUS_TRACED) &&
        (status2 == PEG_STATUS_FILLED || status2 == PEG_STATUS_TRACED))
    {
        return 1;
    }
    return 0;
}

int PegSolitaire::can_receive_left(int index_x, int index_y) const
{
    const int left2_x = index_x - 2;
    if (left2_x < 0)
    {
        return 0;
    }
    const int left2_y = index_y;
    const auto left2_index = index_from_2d(left2_x, left2_y);

    if (left2_index != selected)
    {
        return 0;
    }

    const auto status2 = board.pegs[left2_index];

    const int left1_x = index_x - 1;
    const int left1_y = index_y;
    const auto left1_index = index_from_2d(left1_x, left1_y);
    const auto status1 = board.pegs[left1_index];

    if ((status1 == PEG_STATUS_FILLED || status1 == PEG_STATUS_TRACED) &&
        (status2 == PEG_STATUS_FILLED || status2 == PEG_STATUS_TRACED))
    {
        return 1;
    }
    return 0;
}

int PegSolitaire::can_receive_down(int index_x, int index_y) const
{
    const int down2_x = index_x;
    const int down2_y = index_y + 2;
    if (down2_y >= (int) board.rows)
    {
        return 0;
    }
    const auto down2_index = index_from_2d(down2_x, down2_y);

    if (down2_index != selected)
    {
        return 0;
    }

    const auto status2 = board.pegs[down2_index];

    const int down1_x = index_x;
    const int down1_y = index_y + 1;
    const auto down1_index = index_from_2d(down1_x, down1_y);
    const auto status1 = board.pegs[down1_index];

    if ((status1 == PEG_STATUS_FILLED || status1 == PEG_STATUS_TRACED) &&
        (status2 == PEG_STATUS_FILLED || status2 == PEG_STATUS_TRACED))
    {
        return 1;
    }
    return 0;
}

int PegSolitaire::can_receive_up(int index_x, int index_y) const
{
    const int up2_x = index_x;
    const int up2_y = index_y - 2;
    if (up2_y < 0)
    {
        return 0;
    }
    const auto up2_index = index_from_2d(up2_x, up2_y);

    if (up2_index != selected)
    {
        return 0;
    }

    const auto status2 = board.pegs[up2_index];

    const int up1_x = index_x;
    const int up1_y = index_y - 1;
    const auto up1_index = index_from_2d(up1_x, up1_y);
    const auto status1 = board.pegs[up1_index];

    if ((status1 == PEG_STATUS_FILLED || status1 == PEG_STATUS_TRACED) &&
        (status2 == PEG_STATUS_FILLED || status2 == PEG_STATUS_TRACED))
    {
        return 1;
    }
    return 0;
}
