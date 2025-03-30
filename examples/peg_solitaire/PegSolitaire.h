#pragma once

#include <vector>

typedef struct StepResult
{
    int reward = 0;
    bool game_over = false;
} StepResult;

class PegSolitaire
{
public:

    typedef enum peg_status
    {
        PEG_STATUS_INVALID = 0,
        PEG_STATUS_EMPTY,
        PEG_STATUS_FILLED,
        PEG_STATUS_TRACED,
        //
        PEG_STATUS_COUNT,
    } peg_status;

    typedef enum ps_actions_t
    {
        PS_ACTION_NONE = 0,
        PS_ACTION_LEFT,
        PS_ACTION_RIGHT,
        PS_ACTION_UP,
        PS_ACTION_DOWN,
        PS_ACTION_SPACE,
        // PS_ACTION_COUNT must be last
        PS_ACTION_COUNT
    } ps_actions_t;

    typedef enum board_type
    {
        BOARD_TYPE_ENGLISH,
        BOARD_TYPE_EUROPEAN,
        BOARD_TYPE_GERMAN,
        BOARD_TYPE_ASYMMETRICAL,
        BOARD_TYPE_DIAMOND,
        BOARD_TYPE_TRIANGULAR,
    } board_type;

    typedef enum game_status
    {
        GAME_SELECTION,
        GAME_PLAYING,
        GAME_OVER_LOST,
        GAME_OVER_WON,
    } game_status;

    typedef struct Board
    {
        board_type type{};
        peg_status *pegs{};
        size_t rows{};
        size_t cols{};
        size_t init_col{};
        size_t init_row{};
        size_t size{};
        ~Board();
    } Board;

    StepResult get_step(ps_actions_t action);

    void init_game(board_type type);
    void change_board(board_type type);
    void reset();
    int apply_action(ps_actions_t action);
    void move_cursor_right();
    void move_cursor_left();
    void move_cursor_up();
    void move_cursor_down();
    int check_game_status();
    int update_selected();

    void set_status(game_status status_);
    game_status get_status() const;
    peg_status *get_pegs() const;
    size_t get_cols() const;
    size_t get_rows() const;
    size_t get_cursor() const;
    int get_selected() const;
    size_t count_pegs() const;
    size_t get_state_size() const;
    std::vector<int> get_state() const;

    static size_t index_from_2d(int col, int row, int cols);

    size_t moves{};
    game_status status{};
    size_t selections{};

private:

    Board board{};
    size_t cursor{};
    int selected{};

    int move_peg();
    void reset_board() const;
    void create_board(board_type type);
    int can_move_right(int index_x, int index_y) const;
    int can_move_left(int index_x, int index_y) const;
    int can_move_up(int index_x, int index_y) const;
    int can_move_down(int index_x, int index_y) const;
    int count_peg_moves(int index) const;
};
