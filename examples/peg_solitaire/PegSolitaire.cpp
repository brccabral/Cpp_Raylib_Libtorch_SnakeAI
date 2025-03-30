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

    const int index_x = index % board.cols;
    const int index_y = index / board.cols;

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

int PegSolitaire::check_game_status()
{

    // too many moves without select
    if (moves > 3 * board.size)
    {
        set_status(GAME_OVER_LOST);
        return -1;
    }
    if (selections > 3 * board.size)
    {
        set_status(GAME_OVER_LOST);
        return -1;
    }

    int count_filled = 0;
    int count_traced = 0;
    int count_moveable = 0;
    int index_traced = -1;
    for (size_t i = 0; i < board.size; ++i)
    {
        if (board.pegs[i] == PEG_STATUS_FILLED)
        {
            ++count_filled;
        }
        if (board.pegs[i] == PEG_STATUS_TRACED)
        {
            ++count_traced;
            index_traced = i;
        }
        count_moveable += count_peg_moves(i);
    }

    if (board.type == BOARD_TYPE_EUROPEAN)
    {
        if (count_traced == 0)
        {
            set_status(GAME_OVER_LOST);
            return -1;
        }
    }
    if ((count_filled + count_traced) == 1)
    {
        if (board.type != BOARD_TYPE_EUROPEAN)
        {
            set_status(GAME_OVER_WON);
            return 1;
        }

        if (index_traced != -1)
        {
            const size_t traced_x = index_traced % board.cols;
            const size_t traced_y = index_traced / board.cols;
            if (traced_x == board.init_col && traced_y == board.init_row)
            {
                set_status(GAME_OVER_WON);
                return 1;
            }
        }
    }

    if (count_moveable == 0)
    {
        set_status(GAME_OVER_LOST);
        return -1;
    }

    set_status(GAME_PLAYING);
    return 0;
}

void PegSolitaire::move_cursor_right()
{
    ++moves;

    const int current_x = cursor % board.cols;
    int next_x = current_x;
    const int current_y = cursor / board.cols;
    size_t index;
    do
    {
        next_x = (next_x + 1) % board.cols;
        index = index_from_2d(next_x, current_y);
    }
    while (board.pegs[index] == PEG_STATUS_INVALID);
    cursor = index;
}

void PegSolitaire::move_cursor_left()
{
    ++moves;

    const int current_x = cursor % board.cols;
    int next_x = current_x;
    const int current_y = cursor / board.cols;
    size_t index;
    do
    {
        next_x = ((next_x - 1) % (int) board.cols + board.cols) % board.cols;
        index = index_from_2d(next_x, current_y);
    }
    while (board.pegs[index] == PEG_STATUS_INVALID);
    cursor = index;
}

void PegSolitaire::move_cursor_up()
{
    ++moves;

    const int current_x = cursor % board.cols;
    const int current_y = cursor / board.cols;
    int next_y = current_y;
    size_t index;
    do
    {
        next_y = ((next_y - 1) % (int) board.rows + board.rows) % board.rows;
        index = index_from_2d(current_x, next_y);
    }
    while (board.pegs[index] == PEG_STATUS_INVALID);
    cursor = index;
}

void PegSolitaire::move_cursor_down()
{
    ++moves;

    const int current_x = cursor % board.cols;
    const int current_y = cursor / board.cols;
    int next_y = current_y;
    size_t index;
    do
    {
        next_y = (next_y + 1) % board.rows;
        index = index_from_2d(current_x, next_y);
    }
    while (board.pegs[index] == PEG_STATUS_INVALID);
    cursor = index;
}

int PegSolitaire::move_peg()
{
    // current
    const int cursor_x = cursor % board.cols;
    const int cursor_y = cursor / board.cols;
    // previous
    const int selected_x = selected % board.cols;
    const int selected_y = selected / board.cols;

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
    moves = 0;
    selected = -1;
    return 1;
}

int PegSolitaire::update_selected()
{
    ++selections;
    const peg_status cursor_status = board.pegs[cursor];
    if (cursor_status == PEG_STATUS_FILLED || cursor_status == PEG_STATUS_TRACED)
    {
        if (selected == (int) cursor)
        {
            selected = -1;
            return -1;
        }
        selected = cursor;
        moves = 0;
        return 1;
    }
    if (cursor_status == PEG_STATUS_INVALID)
    {
        selected = -1;
        return -1;
    }

    // cursor is on EMPTY peg

    if (selected == -1)
    {
        return -1;
    }
    // there is one selected
    const peg_status selected_status = board.pegs[selected];
    if (selected_status == PEG_STATUS_INVALID || selected_status == PEG_STATUS_EMPTY)
    {
        selected = -1;
        return -1;
    }

    return move_peg();
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
    moves = 0;
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
    return board.size + 1; // + selected
}

std::vector<double> PegSolitaire::get_state() const
{
    std::vector<double> result(get_state_size(), 0.0);
    // normalize inputs?
    for (size_t p = 0; p < board.size; ++p)
    {
        result[p] = (double) board.pegs[p] / (double) PEG_STATUS_COUNT;
    }
    result[board.size] = (double) selected / board.size;
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
        result.reward = status * 10;
    }

    return result;
}

size_t PegSolitaire::get_action_count() const
{
    return board.size;
}
