#include "pico_graphics.hpp"

namespace pimoroni {
  PicoGraphics::PicoGraphics(uint16_t width, uint16_t height, uint16_t *frame_buffer)
  : frame_buffer(frame_buffer), bounds(0, 0, width, height), clip(0, 0, width, height) {
    set_font(&font8);
  };

  void PicoGraphics::set_font(const Font *font){
    this->font = font;
  }

  void PicoGraphics::set_pen(uint8_t r, uint8_t g, uint8_t b) {
    pen = create_pen(r, g, b);
  }

  void PicoGraphics::set_pen(Pen p) {
    pen = p;
  }

  void PicoGraphics::set_clip(const Rect &r) {
    clip = bounds.intersection(r);
  }

  void PicoGraphics::remove_clip() {
    clip = bounds;
  }

  Pen* PicoGraphics::ptr(const Rect &r) {
    return frame_buffer + r.x + r.y * bounds.w;
  }

  Pen* PicoGraphics::ptr(const Point &p) {
    return frame_buffer + p.x + p.y * bounds.w;
  }

  Pen* PicoGraphics::ptr(int32_t x, int32_t y) {
    return frame_buffer + x + y * bounds.w;
  }

  Pen* PicoGraphics::ptr() {
    return frame_buffer;
  }
  
  void PicoGraphics::clear() {
    rectangle(clip);
  }

  void PicoGraphics::pixel(const Point &p) {
    if(!clip.contains(p)) return;
    *ptr(p) = pen;
  }

  void PicoGraphics::pixel_span(const Point &p, int32_t l) {
    // check if span in bounds
    if( p.x + l < clip.x || p.x >= clip.x + clip.w ||
        p.y     < clip.y || p.y >= clip.y + clip.h) return;

    // clamp span horizontally
    Point clipped = p;
    if(clipped.x     <  clip.x)           {l += clipped.x - clip.x; clipped.x = clip.x;}
    if(clipped.x + l >= clip.x + clip.w)  {l  = clip.x + clip.w - clipped.x;}

    Pen *dest = ptr(clipped);
    while(l--) {
      *dest++ = pen;
    }
  }

  #define StackSize 35400
  #define StoreToStack(x,y) if (stp < StackSize) stack[stp++] = (x & 0xffff) | (y << 16); \
                            *ptr(x,y) = pen;
  
  void PicoGraphics::fill(const Point &p)
  {    
    
    if( p.x < clip.x || p.x >= (clip.x + clip.w) ||
    p.y     < clip.y || p.y >= clip.y + clip.h) return;
    
    uint32_t stp = 0;
    uint32_t stc = 0;
 
    int32_t x = p.x + 1;
    int32_t y = p.y + 1;
    int32_t clipRight = clip.x + clip.w - 1;
    int32_t clipBot = clip.y + clip.h - 1;
    
    
    
    Pen fill_detect_colour = *ptr(p);  //uses the colour at the fill point as the colour to fill
    if (fill_detect_colour == pen) return;
    
    std::vector<uint32_t> stack;
    stack.resize(StackSize);
    
    StoreToStack(x,y);
        
    //return;
    
    while (stc < stp)
    {
        uint32_t p = stack[stc++];
        if (stc >= StackSize) return;
        
        int32_t tx = p & 0xFFFF;
        int32_t ty = p >> 16;
        
        if ((tx > clip.x) && (*ptr(tx - 1, ty) == fill_detect_colour)) {
          StoreToStack(tx - 1, ty);
        }
        if ((tx < clipRight) && (*ptr(tx + 1, ty)  == fill_detect_colour)) {
          StoreToStack(tx + 1, ty);
        }
        if ((ty > clip.y) && (*ptr(tx, ty - 1)  == fill_detect_colour)) {
          StoreToStack(tx, ty - 1);
        }
        if ((ty < clipBot) && (*ptr(tx, ty + 1)  == fill_detect_colour)) {
          StoreToStack(tx, ty + 1);
        }
    }
    
  }
  
  void PicoGraphics::rectangle(const Rect &r) {
    // clip and/or discard depending on rectangle visibility
    Rect clipped = r.intersection(clip);

    if(clipped.empty()) return;

    Pen *dest = ptr(clipped);
    while(clipped.h--) {
      // draw span of pixels for this row
      for(int32_t i = 0; i < clipped.w; i++) {
        *dest++ = pen;
      }

      // move to next scanline
      dest += bounds.w - clipped.w;
    }
  }

  void PicoGraphics::gradientRect(const Rect &rect, std::vector<Colour> cols)
  {
    if (cols.size()<2) return;
    
    float d = rect.h / (cols.size()-1);
    float ry = rect.y;
    
    for (size_t c=0; c < cols.size()-1; c++) {
      Colour &col1 = cols[c], &col2 = cols[c+1];
      
      float r1 = col1.red, g1 = col1.green, b1 = col1.blue;
      float r2 = col2.red, g2 = col2.green, b2 = col2.blue;
      float rd = (r2-r1)/d, gd = (g2-g1)/d, bd = (b2-b1)/d;
      
      for (int32_t y = ry; y < ry + d; y++) {
        set_pen(r1,g1,b1); 
        Pen *dest = ptr(rect.x,y);
        for(int32_t i = 0; i < rect.w; i++) *dest++ = pen;
        r1+=rd; g1+=gd; b1+=bd;
      }
      ry+=d;
    }
    
  }
  
  void PicoGraphics::circle(const Point &p, int32_t radius, uint32_t crescent) {
    // circle in screen bounds?
    Rect bounds = Rect(p.x - radius, p.y - radius, radius * 2, radius * 2);
    if(!bounds.intersects(clip)) return;

    int ox = radius, oy = 0, err = -radius;
    while (ox >= oy)
    {
      int last_oy = oy;

      err += oy; oy++; err += oy;

      pixel_span(Point(p.x - ox, p.y + last_oy), ((ox * crescent)>>16) + 1);
      if (last_oy != 0) {
        pixel_span(Point(p.x - ox, p.y - last_oy), ((ox * crescent)>>16) + 1);
      }

      if(err >= 0 && ox != last_oy) {
        pixel_span(Point(p.x - last_oy, p.y + ox), ((last_oy * crescent)>>16) + 1);
        if (ox != 0) {
          pixel_span(Point(p.x - last_oy, p.y - ox), ((last_oy * crescent)>>16) + 1);
        }

        err -= ox; ox--; err -= ox;
      }
    }
  }


  
  void PicoGraphics::character(const char c, const Point &p, uint8_t scale) {
    uint8_t char_index = c - 32;
    Rect char_bounds(p.x, p.y, font->widths[char_index] * scale, font->height * scale);

    if(!clip.intersects(char_bounds)) return;

    const uint8_t *d = &font->data[char_index * font->max_width];
    for(uint8_t cx = 0; cx < font->widths[char_index]; cx++) {
      for(uint8_t cy = 0; cy < font->height; cy++) {
        if((1U << cy) & *d) {
          rectangle(Rect(p.x + (cx * scale), p.y + (cy * scale), scale, scale));
        }
      }

      d++;
    }
  }


  
  void PicoGraphics::text(const std::string &t, const Point &p, int32_t wrap, uint8_t scale) {
    uint32_t co = 0, lo = 0; // character and line (if wrapping) offset

    size_t i = 0;
    while(i < t.length()) {
      // find length of current word
      size_t next_space = t.find(' ', i + 1);

      if(next_space == std::string::npos) {
        next_space = t.length();
      }

      uint16_t word_width = 0;
      for(size_t j = i; j < next_space; j++) {
        word_width += font->widths[t[j] - 32] * scale;
      }

      // if this word would exceed the wrap limit then
      // move to the next line
      if(co != 0 && co + word_width > (uint32_t)wrap) {
        co = 0;
        lo += (font->height + 1) * scale;
      }

      // draw word
      for(size_t j = i; j < next_space; j++) {
        character(t[j], Point(p.x + co, p.y + lo), scale);
        co += font->widths[t[j] - 32] * scale;
      }

      // move character offset to end of word and add a space
      co += font->widths[0] * scale;
      i = next_space + 1;
    }
  }

  int32_t orient2d(Point p1, Point p2, Point p3) {
    return (p2.x - p1.x) * (p3.y - p1.y) - (p2.y - p1.y) * (p3.x - p1.x);
  }

  bool is_top_left(const Point &p1, const Point &p2) {
    return (p1.y == p2.y && p1.x > p2.x) || (p1.y < p2.y);
  }

  void PicoGraphics::triangle(Point p0, Point p1, Point p2) {
    
    //sort points by y 
    if (p1.y > p2.y) {
      Point sp = p1; p1=p2; p2=sp;
    }
    if (p0.y > p1.y) {
      Point sp = p0; p0=p1; p1=sp;
    }
    if (p1.y > p2.y) {
      Point sp = p1; p1=p2; p2=sp;
    }
    
    if (p2.y<0 || p0.y>bounds.h || p0.y==p2.y) return;
    
    //calc increments (16 fixed point)
    int32_t longEdge = ((p2.x - p0.x)<<16) / (p2.y - p0.y);
    int32_t topEdge = ((p1.x - p0.x)<<16) / ((p1.y == p0.y) ? 1 : p1.y - p0.y);
    int32_t botEdge = ((p2.x - p1.x)<<16) / ((p2.y == p1.y) ? 1 : p2.y - p1.y);
    
    int32_t xf = p0.x << 16;
    int32_t xt = p0.x << 16;
    if (p1.y==p0.y) xt+=topEdge;
    
    //Clip top of triangle if above drawing area
    int32_t ys = p0.y;
    if (ys<0) {
      xf+=longEdge*-p0.y;
      xt+=(p1.y<0) ? topEdge*(p1.y-p0.y)+botEdge*-p1.y : topEdge*-p0.y;
      ys=0;
    }
    
    for (int32_t y=ys; y<=((p2.y >= bounds.h) ? bounds.h-1 : p2.y); y++)
    {
        
      Pen* dest = ptr(0,y);
      if (xf<xt) 
      {
        for (int32_t x = ((xf > 0) ? xf>>16 : 0); x <= ((xt>>16 < bounds.w) ? xt>>16 : bounds.w-1); x++)
          dest[x] = pen;
      }
      else
      {
        for (int32_t x = (xf>>16 < bounds.w ? xf>>16 : bounds.w-1); x >= (xt > 0 ? xt>>16 : 0); x--)
          dest[x] = pen;
      }

      xf+=longEdge;
      xt+=(y < p1.y) ? topEdge : botEdge;
    }
  }

  void PicoGraphics::trilist(const std::vector<int8_t>& tripoints, Point pos, float scale)
  {
    int32_t scl = (int32_t)(scale*65536.f);
    for (size_t i=0; i<tripoints.size(); i+=6) {
      Point p1 = pos + Point((tripoints[i]*scl)>>16,(tripoints[i+1]*scl)>>16);
      Point p2 = pos + Point((tripoints[i+2]*scl)>>16,(tripoints[i+3]*scl)>>16);
      Point p3 = pos + Point((tripoints[i+4]*scl)>>16,(tripoints[i+5]*scl)>>16);
      
      //Point p1 = pos + Point(((tripoints[i]<<16)*scl)>>16,((tripoints[i+1]<<16)*scl)>>16);
      //Point p2 = pos + Point(((tripoints[i+2]<<16)*scl)>>16,((tripoints[i+3]<<16)*scl)>>16);
      //Point p3 = pos + Point(((tripoints[i+4]<<16)*scl)>>16,((tripoints[i+5]<<16)*scl)>>16);
      triangle(p1,p2,p3);
    }
  }

  void PicoGraphics::polygon(const std::vector<Point> &points) {
    static int32_t nodes[64]; // maximum allowed number of nodes per scanline for polygon rendering

    int32_t miny = points[0].y, maxy = points[0].y;

    for (uint16_t i = 1; i < points.size(); i++) {
      miny = std::min(miny, points[i].y);
      maxy = std::max(maxy, points[i].y);
    }

    // for each scanline within the polygon bounds (clipped to clip rect)

    for (int32_t y = std::max(clip.y, miny); y <= std::min(clip.y + clip.h, maxy); y++) {
      uint8_t n = 0;
      for (uint16_t i = 0; i < points.size(); i++) {
        uint16_t j = (i + 1) % points.size();
        int32_t sy = points[i].y;
        int32_t ey = points[j].y;
        if ((sy < y && ey >= y) || (ey < y && sy >= y)) {
          int32_t sx = points[i].x;
          int32_t ex = points[j].x;
          int32_t px = int32_t(sx + float(y - sy) / float(ey - sy) * float(ex - sx));

          nodes[n++] = px < clip.x ? clip.x : (px >= clip.x + clip.w ? clip.x + clip.w - 1 : px);
          if (n>63) break; //avoid crash
        }
      }

      //sort nodes in order
      uint16_t i = 0;
      while (i < n - 1) {
        if (nodes[i] > nodes[i + 1]) {
          int32_t s = nodes[i]; nodes[i] = nodes[i + 1]; nodes[i + 1] = s;
          if (i) i--;
        }
        else {
          i++;
        }
      }

      for (uint16_t i = 0; i < n; i += 2) {
        pixel_span(Point(nodes[i], y), nodes[i + 1] - nodes[i] + 1);
      }
    }
  }

  void PicoGraphics::line(Point p1, Point p2) {
    
    if ((p1.y < 0 && p2.y < 0) || (p1.y >= clip.h && p2.y >= clip.h) ||
        (p1.x < 0 && p2.x < 0) || (p1.x >= clip.w && p2.x >= clip.w)) return;
    
    // fast horizontal line
    if(p1.y == p2.y) {
      int32_t start = std::max(clip.x, std::min(p1.x, p2.x));
      int32_t end   = std::min(clip.x + clip.w, std::max(p1.x, p2.x));
      pixel_span(Point(start, p1.y), end - start);
      return;
    }

    // fast vertical line
    if(p1.x == p2.x) {
      int32_t start  = std::max(clip.y, std::min(p1.y, p2.y));
      int32_t length = std::min(clip.y + clip.h, std::max(p1.y, p2.y)) - start;
      Pen *dest = ptr(p1.x, start);
      while(length--) {
        *dest = pen;
        dest += bounds.w;
      }
      return;
    }

    // general purpose line
    // lines are either "shallow" or "steep" based on whether the x delta
    // is greater than the y delta
    int32_t dx = p2.x - p1.x;
    int32_t dy = p2.y - p1.y;
    bool shallow = std::abs(dx) > std::abs(dy);
    if(shallow) {
      // shallow version
      int32_t s = std::abs(dx);       // number of steps
      int32_t sx = dx < 0 ? -1 : 1;   // x step value
      int32_t sy = (dy << 16) / s;    // y step value in fixed 16:16
      int32_t x = p1.x;
      int32_t y = p1.y << 16;
      while(s--) {
        pixel(Point(x, y >> 16));
        y += sy;
        x += sx;
      }
    }else{
      // steep version
      int32_t s = std::abs(dy);       // number of steps
      int32_t sy = dy < 0 ? -1 : 1;   // y step value
      int32_t sx = (dx << 16) / s;    // x step value in fixed 16:16
      int32_t y = p1.y;
      int32_t x = p1.x << 16;
      while(s--) {
        pixel(Point(x >> 16, y));
        y += sy;
        x += sx;
      }
    }
  }
  
  void PicoGraphics::triTexSection(int32_t &xa, int32_t &xb, int32_t dx1, int32_t dx2, int32_t ya, int32_t yb,
    uint16_t* texture, int32_t tw,
    int32_t &txa, int32_t &tya, int32_t &txb, int32_t &tyb,
    int32_t tdxa, int32_t tdya, int32_t tdxb, int32_t tdyb,
    int32_t sx, int32_t bx)
  {
    for (int32_t y = ya; y != yb; y++)
    {
       int32_t tx = txa;
       int32_t ty = tya;
       
       if ((xb != xa) && !(((xa < sx) && (xb << sx)) || ((xa > bx) && ((xb > bx)))))
       {
         int32_t xt = xb - xa;
         int32_t txd = (txb - txa) / xt;
         int32_t tyd = (tyb - tya) / xt;
         
         int32_t xx = xa >> 16;
         if (xa < sx) {
           tx += txd * (sx-xa);
           ty += tyd * (sx-xa);
           xx = sx;
         }
         
         if (xa > bx) {
           tx += txd * (xa-bx);
           ty += tyd * (xa-bx);
           xx = bx;
         }
         
         xt = xb >> 16;
         if (xb < sx) xt = sx;
         if (xb > bx) xt = bx;
         
         int32_t sg = (xt>xx) ? -1 : 1;
         
         Pen *dest = ptr(xx,y);
         for (int32_t x = xx; x != xt; x+=sg) {
           *dest++ = texture[((ty>>16) * tw) + (tx >> 16)];
           tx += txd;
           ty += tyd;
         }
       }
       
       xa += dx1;
       xb += dx2;
       txa += tdxa;
       tya += tdya;
       txb += tdxb;
       tyb += tdyb;
    }
  }
  
  int32_t getPt(int32_t n1, int32_t n2, int32_t t)
  {
    return n1 + (((n2 - n1) * t)>>16);
  }
  
  Point PicoGraphics::calc_bezier_point(const Point& p1, const Point& p2, const Point& p3, const Point& p4,float p)
  {
    //Bezier blended implementation (FP's are too slow)
    //float ip = 1.f - p;
    //float a0 = ip * ip * ip;
    //float a1 = 3.f * p * ip * ip;
    //float a2 = 3.f * p * p * ip;
    //float a3 = p * p * p;
    //int32_t x = (a0 * p1.x + a1 * p2.x + a2 * p3.x + a3 * p4.x);
    //int32_t y = (a0 * p1.y + a1 * p2.y + a2 * p3.y + a3 * p4.y);
    
    //Fixed point version based on this code
    //https://stackoverflow.com/questions/37642168/how-to-convert-quadratic-bezier-curve-code-into-cubic-bezier-curve/37642695#37642695
    
    int32_t pp = p*(float)(1<<16);
    
    // The Green Line
    int32_t xa = p1.x + (((p2.x - p1.x) * pp)>>16);
    int32_t ya = p1.y + (((p2.y - p1.y) * pp)>>16);
    int32_t xb = getPt( p2.x , p3.x , pp);
    int32_t yb = getPt( p2.y , p3.y , pp);
    int32_t xc = getPt( p3.x , p4.x , pp);
    int32_t yc = getPt( p3.y , p4.y , pp);

    // The Blue Line
    int32_t xm = getPt( xa , xb , pp );
    int32_t ym = getPt( ya , yb , pp );
    int32_t xn = getPt( xb , xc , pp );
    int32_t yn = getPt( yb , yc , pp );

    // The Black Dot
    int32_t x = getPt( xm , xn , pp );
    int32_t y = getPt( ym , yn , pp );
    
    return Point(x,y);
  }
  
  void PicoGraphics::draw_bezier(const Point& p1, const Point& p2, const Point& p3, const Point& p4, int steps)
  {
    float astep = 1.f / (float)steps;
    Point lp = p1;
    for (float i = astep; i<0.999f; i+=astep)
    {
      Point p = calc_bezier_point(p1,p2,p3,p4,i);
      line(lp,p);
      lp = p;
    }
    line(lp,p4);
  }
  
  void PicoGraphics::triangleTexture(Point p1, Point p2, Point p3, 
    uint16_t* texture, int32_t tw, int32_t th,
    Point tp1, Point tp2, Point tp3) {
    
    if (p1.y > p2.y) {
      std::swap(p1,p2);
      std::swap(tp1,tp2);
    }
    
    if (p2.y > p3.y) {
      std::swap(p2,p3);
      std::swap(tp2,tp3);
    }
    
    if (p1.y > p2.y) {
      std::swap(p1,p2);
      std::swap(tp1,tp2);
    }
    
    //determine clipping rectangle (sx,sy,bx,by) ..
    int32_t sy = (p1.y > clip.y) ? p1.y : clip.y;
    int32_t by = (p3.y < clip.y + clip.h) ? p3.y : clip.y + clip.h;
    if (p1.y > by || p3.y < sy || p3.y == p1.y) return;  
    
    int32_t sx = std::min(p1.x, std::min(p2.x, p3.x));
    int32_t bx = std::max(p1.x, std::max(p2.x, p3.x));
    sx = (sx > clip.x) ? sx : clip.x;
    bx = (bx < clip.x + clip.w) ? bx: clip.x + clip.w;

    //work out deltas for each edge (16:16 FP) ..
    int32_t st = (p2.y > p1.y) ? (1 << 16) / ((p2.y - p1.y) << 16) : 1 << 16;
    int32_t dx1 = ((p2.x - p1.x) << 16) * st;
    int32_t tdx1 = ((tp2.x - tp1.x) << 16) * st;
    int32_t tdy1 = ((tp2.y - tp1.y) << 16) * st;
    
    st = (p3.y > p2.y) ? (1 << 16) / ((p3.y - p2.y) << 16) : 1 << 16;
    int32_t dx2 = ((p3.x - p2.x) << 16) * st;
    int32_t tdx2 = ((tp3.x - tp2.x) << 16) * st;
    int32_t tdy2 = ((tp3.y - tp2.y) << 16) * st;
    
    st = (p3.y > p1.y) ? (1 << 16) / ((p3.y - p1.y) << 16) : 1 << 16;
    int32_t dx3 = ((p3.x - p1.x) << 16) * st, dx4 = dx3;
    int32_t tdx3 = ((tp3.x - tp1.x) << 16) * st, tdx4 = tdx3;
    int32_t tdy3 = ((tp3.y - tp1.y) << 16) * st, tdy4 = tdy3;
    
    //Always span left to right ..
    if (dx1 > dx3) {
      std::swap(dx1,dx3);
      std::swap(dx2,dx4);
      std::swap(tdx1,tdx3);
      std::swap(tdy1,tdy3);
      std::swap(tdx2,tdx4);
      std::swap(tdy2,tdy4);
    }
    
    int32_t xa = p1.x, xb = xa;
    int32_t txa = tp1.x << 16, txb = txa;
    int32_t tya = tp1.y << 16, tyb = tya;
    int32_t ys = p1.y;
    int32_t ye = (p2.y<by) ? p2.y : by;
      
    //if p1 and p2 are on the same height, skip top section ...
    if (p2.y==p1.y) {
      if (p1.x > p2.x) {
        xa = p2.x;
        txa = tp2.x << 16;
        tya = tp2.y << 16;
      } else {
        xb = p2.x;
        txb = tp2.x << 16;
        tyb = tp2.y << 16;
      }
    }
    else
    {
      //draw top section ..

      if (p1.y < sy) {
        int32_t d = sy - p1.y;
        xa += dx1 * d;
        txa += tdx1 * d;
        tya += tdy1 * d;
        xb += dx3 * d;
        txb += tdx3 * d;
        tyb += tdy3 * d;
        ys = sy;
      } 
    
      triTexSection(xa, xb, dx1, dx3, ys, ye, texture, tw, txa, tya, txb, tyb, tdx1, tdy1, tdx3, tdy3, sx, bx);
    
      ys = ye;
    }
    
    if ((p3.y - p2.y)<2 || ys == by) return;
    
    if (p2.y < sy) {
        int32_t d = sy - p2.y;
        xa += dx2 * d;
        txa += tdx2 * d;
        tya += tdy2 * d;
        xb += dx4 * d;
        txb += tdx4 * d;
        tyb += tdy4 * d;
        ys = sy;
    }
    ye = (p3.y < by) ? p3.y : by;
    
    //draw bottom section ..
    triTexSection(xa, xb, dx2, dx4, ys, ye, texture, tw, txa, tya, txb, tyb, tdx2, tdy2, tdx4, tdy4, sx, bx);
  }
}
