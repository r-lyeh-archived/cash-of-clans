// simple game economy system. ZLIB/LibPNG licensed.
// - rlyeh ~~ listening to last ninja 2 / matt gray

#include <chrono>
#include <thread>
#include <cstdint>
#include <ctime>

namespace {
    using date = std::uint64_t;

    auto now = []() {
        return std::chrono::steady_clock::now();
    };

    auto timer_app = now();
    date epoch_app = 0;
    date epoch_os = date( std::time(0) );
}

date app_clock( int multiplier = 1 ) {
    using x1 = std::chrono::duration<int, std::ratio<1, 1>>;
    using x2 = std::chrono::duration<int, std::ratio<1, 2>>;
    using x3 = std::chrono::duration<int, std::ratio<1, 3>>;
    using x4 = std::chrono::duration<int, std::ratio<1, 4>>;
    using x5 = std::chrono::duration<int, std::ratio<1, 5>>;
    using x6 = std::chrono::duration<int, std::ratio<1, 6>>;
    using x7 = std::chrono::duration<int, std::ratio<1, 7>>;
    using x8 = std::chrono::duration<int, std::ratio<1, 8>>;
    using x9 = std::chrono::duration<int, std::ratio<1, 9>>;
    using xX = std::chrono::duration<int, std::ratio<1,10>>;

    switch( multiplier ) {
        default: case 1: return epoch_app + std::chrono::duration_cast<x1>( now() - timer_app ).count();
        break;   case 2: return epoch_app + std::chrono::duration_cast<x2>( now() - timer_app ).count();
        break;   case 3: return epoch_app + std::chrono::duration_cast<x3>( now() - timer_app ).count();
        break;   case 4: return epoch_app + std::chrono::duration_cast<x4>( now() - timer_app ).count();
        break;   case 5: return epoch_app + std::chrono::duration_cast<x5>( now() - timer_app ).count();
        break;   case 6: return epoch_app + std::chrono::duration_cast<x6>( now() - timer_app ).count();
        break;   case 7: return epoch_app + std::chrono::duration_cast<x7>( now() - timer_app ).count();
        break;   case 8: return epoch_app + std::chrono::duration_cast<x8>( now() - timer_app ).count();
        break;   case 9: return epoch_app + std::chrono::duration_cast<x9>( now() - timer_app ).count();
        break;  case 10: return epoch_app + std::chrono::duration_cast<xX>( now() - timer_app ).count();
    }
}
date os_clock(int multiplier=1) {
    return epoch_os + app_clock(multiplier);
}
void sleep( double s ) {
    // @todo multiplier
    std::this_thread::sleep_for( std::chrono::milliseconds(int(s * 1000)) );
};
int s( double seconds ) {
    return seconds < 1 ? 1 : seconds;
}
int m( double minutes ) {
    return int( minutes * 60 );
}
int h( double hours ) {
    return int( hours * 3600 );
}
int d( double days ) {
    return int( days * 24 * 3600 );
}
void lapse( int seconds ) {
    epoch_app += seconds;
}

#include <string>
#include <sstream>
#include <cstdlib>

std::string human_time( date t ) {
    date s = t % 60;
    date m = ( t / 60 ) % 60;
    date h = ( t / 3600 ) % 24;
    date d = ( t / 86400 );

    int members = 0;
    char buffer[128], *buf = buffer;
    if( members < 2 ) if( d > 0 ) members++, buf += sprintf( buf, " %dd", d );
    if( members < 2 ) if( h > 0 ) members++, buf += sprintf( buf, " %02dh", h );
    if( members < 2 ) if( m > 0 ) members++, buf += sprintf( buf, " %02dm", m );
    if( members < 2 )             members++, buf += sprintf( buf, " %02ds", s );
    return &buffer[1];
}

#include <vector>
#include <deque>
#include <string>

struct id {
    std::string name;
    unsigned type = 0;
};
#define ID    it.name, it.type

struct cash {
    unsigned gold = 0;
    unsigned elixir = 0;
    unsigned gems = 0;

    bool can_buy( const cash &item ) const {
        if( item.gold > 0 ) if( gold < item.gold ) return false;
        if( item.elixir > 0 ) if( elixir < item.elixir ) return false;
        if( item.gems > 0 ) if( gems < item.gems ) return false;
        return true;
    }
    bool buy( const cash &item ) {
        if( !can_buy(item) ) return false;
        gold -= item.gold;
        elixir -= item.elixir;
        gems -= item.gems;
        return true;
    }
};
#define CASH  it.gold, it.elixir, it.gems

struct purchase {
    cash price;
    date end = 0;

    std::string eta(int multiplier) const {
        date now = os_clock(multiplier);
        if( now < end ) return human_time( end - now );
        return "";
    }
    bool purchased(int multiplier) const {
        date now = os_clock(multiplier);
        return( end <= now );
    }
};
#define PURCHASE it.price, it.end

struct levelup {
    std::vector< purchase > levels;
};
#define LEVELUP it.levels

struct unit : id, cash, purchase, levelup {
};
#define UNIT ID, CASH, PURCHASE, LEVELUP

struct factory : id, cash, purchase, levelup {
    std::deque< unit > units;
    std::deque< unit > in_progress;

    bool can_build( const unit &u ) const {
        return( u.type == type );
    }

    void update( int timefactor ) {
        for( auto it = in_progress.begin(); it != in_progress.end(); ) {
            if( it->eta(timefactor).empty() ) {
                units.push_back( *it );
                it = in_progress.erase( it );
            } else {
                ++it;
            }
        }
    }

    void cancel() {
    }

    std::string debug( int timefactor ) const;
};
#define FACTORY ID, CASH, PURCHASE, LEVELUP, it.units, it.in_progress

#include <algorithm>
struct player : id, cash {

    unsigned playtime = 0;
    unsigned TIMELAPSE_MULTIPLIER = 1;

    std::vector< factory > factories;
    std::vector< factory > in_progress;

    void update( int timefactor ) {
        for( auto it = in_progress.begin(); it != in_progress.end(); ) {
            if( it->eta(timefactor).empty() ) {
                factories.push_back( *it );
                it = in_progress.erase( it );
            } else {
                ++it;
            }
        }
        for( auto &it : factories ) {
            it.update( timefactor );
        }
    }

    void cancel() {
    }

    std::string debug( int timefactor ) const;
};
#define PLAYER ID, CASH, it.playtime, it.TIMELAPSE_MULTIPLIER, it.factories, it.in_progress

// debug & serialization
#include "deps/medea/medea.hpp"

MEDEA_DEFINE( cash &it, (CASH) );
MEDEA_DEFINE( purchase &it, (PURCHASE) );
MEDEA_DEFINE( levelup &it, (LEVELUP) );
MEDEA_DEFINE( unit &it, (UNIT) );
MEDEA_DEFINE( factory &it, (FACTORY) );
MEDEA_DEFINE( player &it, (PLAYER) );

// sample
#include <iostream>
#include <fstream>
#include <functional>
#include <map>

std::string factory::debug( int timefactor ) const {
    std::string names;
    for( auto &unit : units ) {
        names += " " + unit.name;
    }
    std::string etas;
    for( auto &wip : in_progress ) {
        etas += " (" + wip.eta(timefactor) + ")";
    }
    std::stringstream ss;
    ss << name << ": " << units.size() << names << " ; etas:" << etas << std::endl;
    return ss.str();
}
std::string player::debug( int timefactor ) const {
    std::string debugs;
    std::string names;
    for( auto &factory : factories ) {
        names += " " + factory.name;
        debugs += factory.debug(timefactor);
    }
    std::string etas;
    for( auto &wip : in_progress ) {
        etas += " (" + wip.eta(timefactor) + ")";
    }
    std::stringstream ss;
    ss << name << ": " << factories.size() << names << " ; etas:" << etas << std::endl;
    ss << debugs;
    return ss.str();
}

int main() {
    srand(time(0));

    player defaults;

    defaults.name = "r-lyeh";
    defaults.gold = 10000;
    defaults.elixir = 2000;
    defaults.gems = 250;

    player ply = defaults;

    auto random_factory = []() {
        factory f;
        f.type = rand() < RAND_MAX/2;
        if( f.type == 0 )
        f.name = "gold mine #" + std::to_string( 1 + rand() % 10 );
        else
        f.name = "battle squadron #" + std::to_string( 1 + rand() % 10 );
        f.gold = rand() % 100 + 100;
        f.elixir = rand() % 100 + 111;
        f.gems = 0;
        f.end = os_clock() + ( rand() % 15 + 1 ) * ( rand() % 15 + 1 ) * 60;
        return f;
    };
    auto random_unit = []() {
        unit u;
        u.type = rand() < RAND_MAX/2;
        if( u.type == 0 )
        u.name = "miner #" + std::to_string( 1 + rand() % 10 );
        else
        u.name = "soldier #" + std::to_string( 1 + rand() % 10 );
        u.gold = rand() % 10 + 10;
        u.elixir = rand() % 10 + 11;
        u.gems = 0;
        u.end = os_clock() + ( rand() % 15 + 1 ) * ( rand() % 15 + 1 );
        return u;
    };

    auto load = [&]() {
        std::ifstream ifs( "savegame" );
        std::stringstream ss;
        ss << ifs.rdbuf();
        auto copy = ply;
        if( medea::from_json( copy, ss.str() ) ) {
            std::cout << "load ok" << std::endl;
            ply = copy;
        } else {
            std::cout << "unable to load game" << std::endl;
        }
    };
    auto save = [&]() {
        std::ofstream ofs( "savegame" );
        if( ofs << medea::to_json( ply ) ) {
            std::cout << "saved ok" << std::endl;
        } else {
            std::cout << "unable to save game" << std::endl;
        }
    };
    auto status = [&]() {
        std::cout << medea::to_json(ply) << std::endl;
        std::cout << ply.debug(ply.TIMELAPSE_MULTIPLIER) << std::endl;
    };
    auto buy = [&]( const factory &f ) {
        if( !ply.can_buy( f ) ) {
            return;
        }
        ply.in_progress.push_back( f );
        ply.buy( f );
        save();
    };
    auto build = [&]( const unit &u ) {
        if( !ply.can_buy( u ) ) {
            return;
        }
        for( auto &f : ply.factories ) {
            if( f.can_build( u ) ) {
                f.in_progress.push_back( u );
                ply.buy( u );
                save();
                return;
            }
        }
    };
    auto cancel = [&]() {
        ply.in_progress.clear();
        // todo: refund
        save();
    };
    auto clear = [&]() {
        ply = defaults;
        save();
    };

    load();
    save();

    std::map< std::string, std::function<void()> > bind;
    std::map< std::string, std::string > alias;

    bind[ alias["b"] = "buy" ] = [&](){
        buy( random_factory() );
    };

    bind[ alias["m"] = "make" ] = [&]() {
        build( random_unit() );
    };

    bind[ alias["s"] = alias["st"] = "status" ] = [&]() {
        status();
    };

    bind[ alias["h"] = alias["?"] = "help" ] = [&]() {
        for( auto &in : bind ) {
            std::string found;
            for( auto &in2 : alias ) {
                if( in.first == in2.second ) {
                    found = in2.first;
                }
            }
            if( found.empty() )
            std::cout << in.first << ", ";
            else
            std::cout << in.first << "(" << found << "), ";
        }
        std::cout << std::endl;
    };

    bind[ alias["c"] = "cancel" ] = [&]() {
        cancel();
    };

    bind[ alias["cl"] = "clear" ] = [&]() {
        clear();
    };

    bind[ alias["s"] = "save" ] = [&]() {
        save();
    };

    bind[ alias["l"] = "load" ] = [&]() {
        load();
    };

    bind[ "5m" ] = [&]() {
        lapse( m(5) );
    };
    bind[ "15m" ] = [&]() {
        lapse( m(15) );
    };
    bind[ "1h" ] = [&]() {
        lapse( h(1) );
    };
    bind[ alias["1d"] = "24h" ] = [&]() {
        lapse( h(24) );
    };

    // cheats and/or powerups
    bind[ alias["-"] = "--" ] = [&]() {
        ply.gold /= 2;
        ply.elixir /= 2;
        ply.gems /= 2;
    };
    bind[ alias["+"] = "++" ] = [&]() {
        ply.gold += 1 + ply.gold/2;
        ply.elixir += 1 + ply.elixir/2;
        ply.gems += 1 + ply.gems/2;
    };
    bind[ "x1" ] = [&]() {
        // @todo, meter 15m&&co en una func() y avanzar el tiempo de eta con ella
        // for all building items
        // get eta
        ply.TIMELAPSE_MULTIPLIER = 1;
        // remaining = now + eta
    };
    bind[ "x2" ] = [&]() {
        ply.TIMELAPSE_MULTIPLIER = 2;
    };
    bind[ "x3" ] = [&]() {
        ply.TIMELAPSE_MULTIPLIER = 3;
    };
    bind[ "x4" ] = [&]() {
        ply.TIMELAPSE_MULTIPLIER = 4;
    };
    bind[ "x5" ] = [&]() {
        ply.TIMELAPSE_MULTIPLIER = 5;
    };
    bind[ "x6" ] = [&]() {
        ply.TIMELAPSE_MULTIPLIER = 6;
    };
    bind[ "x7" ] = [&]() {
        ply.TIMELAPSE_MULTIPLIER = 7;
    };
    bind[ "x8" ] = [&]() {
        ply.TIMELAPSE_MULTIPLIER = 8;
    };
    bind[ "x9" ] = [&]() {
        ply.TIMELAPSE_MULTIPLIER = 9;
    };
    bind[ "x10" ] = [&]() {
        ply.TIMELAPSE_MULTIPLIER = 10;
    };

    bind[ alias["q"] = "quit" ] = [&]() {
        unsigned session_time = app_clock(1);
        ply.playtime += session_time;
        save();
        std::cout << "session: " << human_time(session_time) << "; total: " << human_time(ply.playtime) << std::endl;
        std::exit(0);
    };


    while(1) {

        ply.update(ply.TIMELAPSE_MULTIPLIER);

        std::cout << "prompt>";
        std::string cmd;
        std::getline( std::cin, cmd );

        // game
        if( cmd.empty() ) {
            cmd = "status";
        }

        /**/ if( bind.find(cmd) != bind.end() ) {
            bind[cmd]();
        }
        else if( alias.find(cmd) != alias.end() ) {
            bind[alias[cmd]]();
        }
        else {
            std::cout << "??" << std::endl;
        }

    }

}
