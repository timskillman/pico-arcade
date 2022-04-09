#include "pico_widgets.hpp"
#include <cmath>
namespace pimoroni {
	
	#define segsize 3
	void draw_segment(PicoDisplay &pico_display, uint8_t num, int x, int y, int size, uint16_t col1, uint16_t col2)
	{
		
		static uint8_t seg[11] = { 0b1110111, 0b0100100, 0b1011101, 0b1101101, 0b0101110, 0b1101011, 0b1111011, 0b0100101, 0b1111111, 0b1101111, 0b0000000 };
		
		uint8_t n=num % 11;
		//top
		pico_display.set_pen((seg[n] & 1) ? col1 : col2);
		pico_display.rectangle(Rect(x+size,y,size*segsize,size));
		//top-left
		pico_display.set_pen((seg[n] & 2) ? col1 : col2);
		pico_display.rectangle(Rect(x,y+size,size,size*segsize));
		//top-right
		pico_display.set_pen((seg[n] & 4) ? col1 : col2);
		pico_display.rectangle(Rect(x+size+size*segsize,y+size,size,size*segsize));
		//middle
		pico_display.set_pen((seg[n] & 8) ? col1 : col2);
		pico_display.rectangle(Rect(x+size,y+size+size*segsize,size*segsize,size));
		//bot-left
		pico_display.set_pen((seg[n] & 16) ? col1 : col2);
		pico_display.rectangle(Rect(x,y+size*2+size*segsize,size,size*segsize));
		//bot-right
		pico_display.set_pen((seg[n] & 32) ? col1 : col2);
		pico_display.rectangle(Rect(x+size+size*segsize,y+size*2+size*segsize,size,size*segsize));
		//bot
		pico_display.set_pen((seg[n] & 64) ? col1 : col2);
		pico_display.rectangle(Rect(x+size,y+size*2+size*segsize*2,size*segsize,size));
	}

	void draw_sevensegs(PicoDisplay &pico_display, const Point& pos, int num, int digits, int size, uint16_t col1, uint16_t col2)
	{
		std::string nstr = std::to_string(num);
		for (int d=0; d<nstr.length(); d++) {
			int8_t n=(int8_t)(nstr[d]-'0');
			draw_segment(pico_display, n, pos.x+d*(size*segsize*2), pos.y, size, col1, col2);
		}
	}
	
	void draw_measurements(PicoDisplay &pico_display, const Rect& rect, int value, int ticks, int tick_dist, int tick_length, int tick_thick, int tick_major, int tick_major_length, Pen col, std::vector<Colour>& cols)
	{
		Point pos(rect.x,rect.y);
		
		if (cols.size()>0) {
			pico_display.gradientRectRounded(rect,0,cols);
		}
		
		pico_display.set_pen(col);
		
		int maxw=(ticks*tick_dist > rect.w) ? rect.w : ticks*tick_dist;
		pico_display.line(pos,pos+Point(maxw,0));
		
		for (int t=0; t<ticks && (t*tick_dist < rect.w); t++) {
			pico_display.rectangle(Rect(pos.x + t*tick_dist, pos.y, tick_thick, (t % tick_major) ? tick_length : tick_major_length));
			if (t %  tick_major == 0) pico_display.text(std::to_string(t),Point(pos.x+t*tick_dist-4,pos.y+tick_major_length),255,2);
		}
		
		Point ptop = pos;
		ptop = ptop + Point(value*tick_dist,tick_major_length+16);
		pico_display.triangle(ptop,ptop+Point(-6,12),ptop+Point(6,12));
	}
	
	void draw_meter(PicoDisplay &pico_display, const Rect& rect, const Point& center, int radius, int value, int start_angle, int end_angle, int tick_dist, int tick_length, int tick_thick, int tick_major, int tick_major_length, Pen col, std::vector<Colour>& cols)
	{
		if (cols.size()>0) {
			pico_display.gradientRectRounded(rect,20,cols);
		}
		
		Point pos(rect.x,rect.y);
		Point ce = pos+center;
		
		
		Pen grn = pico_display.create_pen(0,255,0);
		pico_display.set_pen(col);
		Point op1, op2;
		
		pico_display.set_pen(grn);
		for (int a=start_angle; a<end_angle; a+=tick_dist) {
			int32_t s = fastsin(a+540), c = fastcos(a+540);
			Point p1((-radius* s)>>16, (radius * c)>>16);
			Point p2((-(radius-tick_length)* s)>>16, ((radius-tick_length) * c)>>16);
			if (a!=start_angle) {
				pico_display.triangle(ce+op1,ce+p1,ce+op2);
				pico_display.triangle(ce+op2,ce+p1,ce+p2);
			}
			op1=p1;
			op2=p2;
		}
		
		int tm = 0;
		pico_display.set_pen(col);
		for (int a=start_angle; a<end_angle; a+=tick_dist) {
			int32_t s = fastsin(a+540), c = fastcos(a+540);
			Point p1((-radius* s)>>16, (radius * c)>>16);
			int32_t tl = (tm % tick_major) ? tick_length : tick_major_length;
			Point p2((-(radius-tl)* s)>>16, ((radius-tl) * c)>>16);
			
			if (a!=start_angle) {
				pico_display.line(ce + op1, ce + p1);
			}
			pico_display.line(ce + p1,ce + p2);
			
			op1=p1;
			tm++;
		}
		

		
		int32_t v=(int32_t)(((float)(end_angle-start_angle)/256)*(float)value);
		int s = fastsin(start_angle+v+540), c = fastcos(start_angle+v+540);
		Point np((-(radius-tick_major_length)* s)>>16, ((radius-tick_major_length) * c)>>16);
		s = fastsin(start_angle+v+450); c = fastcos(start_angle+v+450);
		Point fo((-tick_major_length * s)>>16, (tick_major_length * c)>>16);
		
		pico_display.triangle(ce+np,ce-fo,ce+fo);
		//pico_display.line(ce,ce+np);
	}
}
