/******************* STATISTICS *******************/

#include <stdlib.h>

#include "display.h"
#include "main.h"
#include "util.h"

#define STAT_PACK_POS 9
#define STAT_BYTE_POS (STAT_PACK_POS + 9)
#define STAT_BPP_POS (STAT_BYTE_POS + 9)
#define STAT_PP_POS (STAT_BPP_POS + 6)
#define STAT_BP_POS (STAT_PP_POS + 6)
#define STAT_AIR_POS (STAT_BP_POS + 6)
#define STAT_AIRG_POS (STAT_AIR_POS + 6)


void
update_statistics_win(WINDOW *win)
{
	int i;
	int line;
	int bps, dps;
	float duration;

	werase(win);
	wattron(win, WHITE);
	box(win, 0 , 0);
	print_centered(win, 0, COLS, " Packet Statistics ");

	if (stats.packets == 0) {
		wnoutrefresh(win);
		return; /* avoid floating point exceptions */
	}

	mvwprintw(win, 2, 2, "Packets: %d",stats.packets );
	mvwprintw(win, 3, 2, "Bytes:   %s (%d)",  kilo_mega_ize(stats.bytes), stats.bytes );
	mvwprintw(win, 4, 2, "Average: ~%d B/Pkt", stats.bytes/stats.packets);

	get_per_second(stats.bytes, stats.duration, &bps, &dps);
	bps = bps * 8;

	mvwprintw(win, 2, 40, "Total bit/sec: %s (%d)", kilo_mega_ize(bps), bps);

	wattron(win, A_BOLD);
	mvwprintw(win, 3, 40, "Total Usage:   %3.1f%% (%d)", dps * 1.0 / 10000, dps ); /* usec in % */
	wattroff(win, A_BOLD);

	line = 6;
	mvwprintw(win, line, STAT_PACK_POS, " Packets");
	mvwprintw(win, line, STAT_BYTE_POS, "   Bytes");
	mvwprintw(win, line, STAT_BPP_POS, "~B/P");
	mvwprintw(win, line, STAT_PP_POS, "Pkts%%");
	mvwprintw(win, line, STAT_BP_POS, "Byte%%");
	wattron(win, A_BOLD);
	mvwprintw(win, line, STAT_AIR_POS, "Usage%%");
	mvwprintw(win, line++, 2, "RATE");
	wattroff(win, A_BOLD);
	mvwhline(win, line++, 2, '-', COLS-4);
	for (i = 1; i < MAX_RATES; i++) {
		if (stats.packets_per_rate[i] > 0) {
			wattron(win, A_BOLD);
			mvwprintw(win, line, 2, "%3dM", i/2);
			wattroff(win, A_BOLD);
			mvwprintw(win, line, STAT_PACK_POS, "%8d",
				stats.packets_per_rate[i]);
			mvwprintw(win, line, STAT_BYTE_POS, "%8s",
				kilo_mega_ize(stats.bytes_per_rate[i]));
			mvwprintw(win, line, STAT_BPP_POS, "%4d",
				stats.bytes_per_rate[i] / stats.packets_per_rate[i]);
			mvwprintw(win, line, STAT_PP_POS, "%2.1f",
				stats.packets_per_rate[i] * 100.0 / stats.packets);
			mvwprintw(win, line, STAT_BP_POS, "%2.1f",
				stats.bytes_per_rate[i] * 100.0 / stats.bytes);
			wattron(win, A_BOLD);
			duration = stats.duration_per_rate[i] * 100.0 / stats.duration;
			mvwprintw(win, line, STAT_AIR_POS, "%2.1f", duration);
			mvwhline(win, line, STAT_AIRG_POS, '*',
				normalize(duration, 100, COLS - STAT_AIRG_POS - 2));
			wattroff(win, A_BOLD);
			line++;
		}
	}

	line++;
	mvwprintw(win, line, STAT_PACK_POS, " Packets");
	mvwprintw(win, line, STAT_BYTE_POS, "   Bytes");
	mvwprintw(win, line, STAT_BPP_POS, "~B/P");
	mvwprintw(win, line, STAT_PP_POS, "Pkts%%");
	mvwprintw(win, line, STAT_BP_POS, "Byte%%");
	wattron(win, A_BOLD);
	mvwprintw(win, line, STAT_AIR_POS, "Usage%%");
	mvwprintw(win, line++, 2, "TYPE");
	wattroff(win, A_BOLD);
	mvwhline(win, line++, 2, '-', COLS - 4);
	for (i = 0; i < MAX_FSTYPE; i++) {
		if (stats.packets_per_type[i] > 0) {
			wattron(win, A_BOLD);
			mvwprintw(win, line, 2, "%s", get_packet_type_name(i));
			wattroff(win, A_BOLD);
			mvwprintw(win, line, STAT_PACK_POS, "%8d",
				stats.packets_per_type[i]);
			mvwprintw(win, line, STAT_BYTE_POS, "%8s",
				kilo_mega_ize(stats.bytes_per_type[i]));
			mvwprintw(win, line, STAT_BPP_POS, "%4d",
				stats.bytes_per_type[i] / stats.packets_per_type[i]);
			mvwprintw(win, line, STAT_PP_POS, "%2.1f",
				stats.packets_per_type[i] * 100.0 / stats.packets);
			mvwprintw(win, line, STAT_BP_POS, "%2.1f",
				stats.bytes_per_type[i] * 100.0 / stats.bytes);
			wattron(win, A_BOLD);
			if (stats.duration > 0)
				duration = stats.duration_per_type[i] * 100.0 / stats.duration;
			else
				duration = 100.0;
			mvwprintw(win, line, STAT_AIR_POS, "%2.1f", duration);
			mvwhline(win, line, STAT_AIRG_POS, '*',
				normalize(duration, 100, COLS - STAT_AIRG_POS - 2));
			wattroff(win, A_BOLD);
			line++;
		}
	}
	wnoutrefresh(win);
}
