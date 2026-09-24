/* Copyright (C) 2019 Mr Goldberg
   This file is part of the Goldberg Emulator

   The Goldberg Emulator is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 3 of the License, or (at your option) any later version.

   The Goldberg Emulator is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the Goldberg Emulator; if not, see
   <http://www.gnu.org/licenses/>.  */

#pragma once

#include "local_storage.h"
#include <cstdint>
#include <chrono>
#include <mutex>
#include <string>

class Settings; // forward declare

class PlaytimeCounter {
public:
    // было: (Local_Storage*, bool record_playtime = false)
    // стало: опционально Settings* для API (record_playtime берём из settings, если передан)
    explicit PlaytimeCounter(Local_Storage* local_storage, bool record_playtime = false, Settings* settings = nullptr);
    ~PlaytimeCounter();

    void tick();
    void load();
    void save();

    uint64_t seconds() const;
    uint64_t session_seconds() const;

    void set_pause_total(bool pause);
    void set_pause_session(bool pause);

    bool get_record_playtime() const { return record_playtime; }

private:
    void send_to_api();   // <-- новое

    Local_Storage* local_storage{};
    Settings* settings{}; // <-- новое (может быть nullptr)
    bool record_playtime = false;
    const std::string playtime_filename = "playtime.txt";
    std::chrono::steady_clock::time_point last_tick{};
    uint64_t playtime_seconds = 0;
    uint64_t playtime_accumulator_ms = 0;
    uint64_t session_seconds_accumulated = 0;
    bool pause_total = false;
    bool pause_session = false;
    mutable std::mutex mutex;
    bool initialized = false;
    uint64_t since_save = 0;
};
