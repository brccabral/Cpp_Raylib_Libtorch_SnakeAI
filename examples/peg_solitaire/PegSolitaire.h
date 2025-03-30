#pragma once

#include <mlgames/AbstractGame.h>

class PegSolitaire : public AbstractGame
{
public:

    size_t get_state_size() const override;
    size_t get_action_count() const override;
    int get_score() const override;
    StepResult get_step(int index) override;
    std::vector<int> get_state() const override;
    void reset() override;

    typedef enum peg_status
    {
        PEG_STATUS_INVALID = 0,
        PEG_STATUS_EMPTY,
        PEG_STATUS_FILLED,
        PEG_STATUS_TRACED,
        //
        PEG_STATUS_COUNT,
    } peg_status;

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


    void init_game(board_type type);
    void change_board(board_type type);
    void move_cursor_right();
    void move_cursor_left();
    void move_cursor_up();
    void move_cursor_down();
    int check_game_status();
    [[nodiscard]] int update_selected();

    void set_status(game_status status_);
    game_status get_status() const;
    peg_status *get_pegs() const;
    size_t get_cols() const;
    size_t get_rows() const;
    size_t get_cursor() const;
    int get_selected() const;

    size_t index_from_2d(int col, int row) const;

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
