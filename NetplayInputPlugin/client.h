#pragma once

#include "stdafx.h"

#include <fstream>
#include <cryptopp/sha.h>
#include <cryptopp/files.h>
#include <cryptopp/filters.h>
#include <cryptopp/hex.h>

#include "connection.h"
#include "packet.h"
#include "Controller_1.1.h"
#include "common.h"
#include "client_dialog.h"
#include "server.h"

int get_input_rate(country_code code);

class client: public service_wrapper, public connection {
    public:
        client(std::shared_ptr<client_dialog> dialog);
        ~client();
        void load_public_server_list();
        void get_external_address();
        std::string get_name();
        void set_name(const std::string& name);
        void set_rom_info(const rom_info& rom);
        void set_src_controllers(CONTROL controllers[4]);
        void set_dst_controllers(CONTROL controllers[4]);
        void process_input(std::array<BUTTONS, 4>& input);
        bool wait_until_start();
        void post_close();
        client_dialog& get_dialog();
        std::string get_favorite_server();
        void set_favorite_server(const std::string& fav_server);
        void set_save_info(const std::string& save_path);
        virtual void on_receive(packet& packet, bool udp);
        virtual void on_error(const std::error_code& error);
    private:
        constexpr static uint32_t MARIO_GOLF_MASK = 0xFFFFF0F0;

        asio::steady_timer timer;
        bool started = false;
        std::mutex start_mutex;
        std::condition_variable start_condition;
        std::mutex next_input_mutex;
        std::condition_variable next_input_condition;
        std::list<std::array<BUTTONS, 4>> next_input;
        uint32_t input_id = 0;
        std::list<double> input_times;
        bool golf = false;
        std::string host;
        uint16_t port;
        std::string path;
        std::string save_path;
        std::shared_ptr<user_info> me = std::make_shared<user_info>();
        std::vector<std::shared_ptr<user_info>> user_map = { me };
        std::vector<std::shared_ptr<user_info>> user_list = { me };
        std::map<std::string, double> public_servers;
        CONTROL* controllers;
        std::shared_ptr<client_dialog> my_dialog;
        std::shared_ptr<server> my_server;
        bool frame_limit = true;
        HANDLE qos_handle = NULL;
        bool upnp_active = false;
        uint16_t upnp_port = 0;
#ifdef DEBUG
        std::ofstream input_log;
#endif

        static bool input_detected(const input_data& input, uint32_t mask = 0xFFFFFFFF);

        virtual void close(const std::error_code& error = std::error_code());
        void ping_public_server_list();
        void start_game();
        void on_message(std::string message);
        void set_lag(uint8_t lag);
        void message_received(uint32_t id, const std::string& message);
        void remove_user(uint32_t id);
        void connect(const std::string& host, uint16_t port, const std::string& room);
        void map_src_to_dst();
        void on_input();
        void on_tick();
        void update_user_list();
        void refresh_button_states();
        bool check_rom_match();
        bool check_saves_match();
        void set_input_authority(application auth);
        void set_input_map(input_map map);
        void set_golf_mode(bool golf);
        void send_join(const std::string& room, uint16_t udp_port);
        void send_name();
        void send_controllers();
        void send_message(const std::string& message);
        void send_start_game();
        void send_lag(uint8_t lag, bool my_lag, bool your_lag);
        void send_autolag(int8_t value = -1);
        void send_input(const input_data& input);
        void send_hia_input(const input_data& input);
        void send_input_map(input_map map);
        void send_input_rate(float rate);
        void send_udp_ping();
        void update_save_info();
        std::vector<std::string> find_rom_save_files();
        std::string sha256_file(const std::string& filepath);
        std::string slurp_binary(const std::string& filepath);
        void replace_save_file(const save_info& save);
        void send_save_info();
        size_t send_savesync(const std::string& target_name = "");
        void cleanup_upnp();
};
