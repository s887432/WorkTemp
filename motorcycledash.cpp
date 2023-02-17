/*
 * Copyright (C) 2018 Microchip Technology Inc.  All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <egt/ui>
#include <memory>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <sys/time.h>
#include <random>
#include <chrono>
#include <queue>
#include <poll.h>
#include <sys/socket.h>
#include <linux/gpio.h>

#define HAVE_LIBPLANES

#ifdef HAVE_LIBPLANES
#include <planes/plane.h>
#include "egt/detail/screen/kmsoverlay.h"
#endif
#include "../src/detail/erawimage.h"
#include "../src/detail/eraw.h"


#define DO_SVG_SERIALIZATION
#define ENABLE_CAMERA_SUPPORT
#define USE_KEY_EVENT
//#define USE_GPIO_EVENT

#define MEDIA_FILE_PATH "/root/"
#define solve_relative_path(relative_path) (MEDIA_FILE_PATH + relative_path)

//If this macro defined, scale animation will scale from min to max and stay, then enter to demo.
//Else if not defined, scale animation will scale from min to max, and then scale to min, after that enter to demo.
#define SCALE_TO_MAX_AND_STAY

#define HIGH_Q_TIME_THRESHHOLD 30000  //30ms
#define LOW_Q_TIME_THRESHHOLD  20000  //20ms

#define ID_MAX               1327
#define ID_MIN               847
#define STEPPER              4
#define PLANE_WIDTH_HF       400
#define PLANE_HEIGHT_HF      240
#define MAX_NEEDLE_INDEX     120

int g_digit = ID_MIN;
bool is_increasing = true;

enum class SvgEleType
{
    path = 0,
    text,
};

#ifdef USE_GPIO_EVENT

// PB14
#define GPIO_SET_REVERSE	"/dev/gpiochip1"
#define GPIO_PIN_REVERSE	14

class GpioEventMonitor
{
public:

    using Callback = std::function<void(gpioevent_data& event)>;

    GpioEventMonitor() = delete;

    GpioEventMonitor(int fd, Callback callback)
        : input(egt::Application::instance().event().io()),
		callback(std::move(callback)),
		buffer(1)
    {
        // assign fd to input stream
        input.assign(fd);

        // register first async read
        egt::asio::async_read(input, egt::asio::buffer(buffer),
			std::bind(&GpioEventMonitor::handle_read,
			this,
			std::placeholders::_1));
    }

    void handle_read(const egt::asio::error_code& error)
    {
        if (error)
        {
            std::cout << "handle_read: " << error.message() << std::endl;
            return;
        }

        callback(buffer.front());

        // register any additional async read
        egt::asio::async_read(input, egt::asio::buffer(buffer),
			std::bind(&GpioEventMonitor::handle_read,
			this,
			std::placeholders::_1));
    }

private:
    // input wrapper around the fd
    egt::asio::posix::stream_descriptor input;
    // registered callback
    Callback callback;
    // buffer to hold the gpioevent_data
    std::vector<gpioevent_data> buffer;
};
#endif

class MotorDash : public egt::experimental::Gauge
{
public:

#ifdef DO_SVG_SERIALIZATION
    MotorDash(egt::SvgImage& svg) noexcept: m_svg(svg)
#else
    MotorDash() noexcept
#endif
    {
        set_needle_deserial_state(false);
        set_speed_deserial_state(false);
        set_blink_deserial_state(false);
        set_tc_deserial_state(false);
        set_high_q_state(true);
        set_low_q_state(true);
    }

#ifdef DO_SVG_SERIALIZATION
    void serialize_all();

    void render_needles()
    {
        for (int i = ID_MIN; i <= ID_MAX; i += STEPPER)
        {
            add_svg_layer_with_digit(m_svg, "#path", i, false);
        }
    }

    void render_needles(int render_start, int render_end)
    {
        for (int i = render_start; i <= render_end; i += STEPPER)
        {
            add_svg_layer_with_digit(m_svg, "#path", i, false);
        }
    }

    void render_needles(int render_digit)
    {
        add_svg_layer_with_digit(m_svg, "#path", render_digit, true);
    }
#endif

    void add_text_widget(const std::string& id, const std::string& txt, const egt::Rect& rect, egt::Font::Size size)
    {
        auto text = std::make_shared<egt::TextBox>(txt, rect, egt::AlignFlag::center);
        text->border(0);
        text->font(egt::Font(size, egt::Font::Weight::normal));
        text->color(egt::Palette::ColorId::bg, egt::Palette::transparent);
        text->color(egt::Palette::ColorId::text, egt::Palette::white);
        add(text);
        text->name(id);
    }

    void add_text_widget(const std::string& id, const std::string& txt, const egt::Rect& rect, egt::Font::Size size, egt::Color color)
    {
        auto text = std::make_shared<egt::TextBox>(txt, rect);
        text->border(0);
        text->font(egt::Font(size, egt::Font::Weight::normal));
        text->color(egt::Palette::ColorId::bg, egt::Palette::transparent);
        text->color(egt::Palette::ColorId::text, color);
        add(text);
        text->name(id);
    }

#ifdef DO_SVG_SERIALIZATION
    void add_rec_widget(const std::string& id, bool is_hiding)
    {
        if (!m_svg.id_exists(id))
            return;

        auto box = m_svg.id_box(id);
        auto rec = std::make_shared<egt::RectangleWidget>(egt::Rect(std::floor(box.x()),
                                                                    std::floor(box.y()),
                                                                    std::ceil(box.width()),
                                                                    std::ceil(box.height())));
        rec->color(egt::Palette::ColorId::button_bg, egt::Palette::black);
        if (is_hiding)
            rec->hide();
        add(rec);
        rec->name(id);
    }
#endif

    std::shared_ptr<egt::TextBox> find_text(const std::string& name)
    {
        return find_child<egt::TextBox>(name);
    }

    std::shared_ptr<egt::RectangleWidget> find_rec(const std::string& name)
    {
        return find_child<egt::RectangleWidget>(name);
    }

    std::shared_ptr<egt::experimental::GaugeLayer> find_layer(const std::string& name)
    {
        return find_child<egt::experimental::GaugeLayer>(name);
    }

    void set_text(const std::string& id, const std::string& text, int font_size, const egt::Pattern& color)
    {
        auto layer = find_rec(id);
        auto ptext = std::make_shared<egt::Label>();
        ptext->text_align(egt::AlignFlag::center);
        ptext->box(egt::Rect(layer->box().x(), layer->box().y(), layer->box().width(), layer->box().height()));
        ptext->color(egt::Palette::ColorId::label_text, color);
        ptext->font(egt::Font(font_size, egt::Font::Weight::normal));
        ptext->text(text);
        add(ptext);
    }

    void set_color(const egt::Color& color)
    {
        for (auto i = ID_MIN; i <= ID_MAX; i += STEPPER)
        {
            std::ostringstream ss;
            ss << "#path" << std::to_string(i);
            find_layer(ss.str())->mask_color(color);
        }
    }

    void show_digit(int digit)
    {
        std::ostringstream ss;
        ss << "#path" << std::to_string(digit);
        //std::cout << "show_digit: " << ss.str() << std::endl;
        find_layer(ss.str())->show();
    }

    void hide_all()
    {
        for (auto& child : m_children)
        {
            if (child->name().rfind("#path", 0) == 0)
                child->hide();
        }
    }

    bool get_high_q_state() { return m_is_high_q_quit; }
    void set_high_q_state(bool is_high_q_quit) { m_is_high_q_quit = is_high_q_quit; }

    bool get_low_q_state() { return m_is_low_q_quit; }
    void set_low_q_state(bool is_low_q_quit) { m_is_low_q_quit = is_low_q_quit; }

    bool get_needle_deserial_state() { return m_needle_deserial_finish; }
    void set_needle_deserial_state(bool needle_deserial_finish) { m_needle_deserial_finish = needle_deserial_finish; }

    bool get_speed_deserial_state() { return m_speed_deserial_finish; }
    void set_speed_deserial_state(bool speed_deserial_finish) { m_speed_deserial_finish = speed_deserial_finish; }

    bool get_blink_deserial_state() { return m_blink_deserial_finish; }
    void set_blink_deserial_state(bool blink_deserial_finish) { m_blink_deserial_finish = blink_deserial_finish; }

    bool get_tc_deserial_state() { return m_tc_deserial_finish; }
    void set_tc_deserial_state(bool tc_deserial_finish) { m_tc_deserial_finish = tc_deserial_finish; }

    //egt::Application *app_ptr = nullptr;
    int needle_digit = ID_MIN;

    egt::shared_cairo_surface_t DeSerialize(const std::string& filename, std::shared_ptr<egt::Rect>& rect);
    void ConvertInkscapeRect2EGT(std::shared_ptr<egt::Rect>& rect);

#ifdef DO_SVG_SERIALIZATION
    void SerializeSVG(const std::string& filename, egt::SvgImage& svg, const std::string& id);
    void SerializePNG(const char* png_src, const std::string& png_dst);
#endif

private:
#ifdef DO_SVG_SERIALIZATION
    void add_svg_layer(egt::SvgImage& svg, const std::string& ss, bool is_hiding)
    {
        if (nullptr != find_layer(ss))
            return;
        auto layer = std::make_shared<egt::experimental::GaugeLayer>(svg.render(ss));
        if (is_hiding)
            layer->hide();
        add(layer);
        layer->name(ss);
        //std::cout << "render widget:" << ss << std::endl;
    }

    void add_svg_layer_with_digit(egt::SvgImage& svg, const std::string& ss, int digit, bool is_hiding)
    {
        std::ostringstream str;
        str << ss << std::to_string(digit);
        if (nullptr != find_layer(str.str()))
            return;
        auto box = svg.id_box(str.str());
        auto layer = std::make_shared<egt::experimental::GaugeLayer>(svg.render(str.str(), box));

        layer->name(str.str());
        layer->box(egt::Rect(box.x() + moat(),
                                box.y() + moat(),
                                std::ceil(box.width()),
                                std::ceil(box.height())));
        if (is_hiding)
            layer->hide();
        add(layer);
        needle_digit += STEPPER;
    }
    egt::SvgImage& m_svg;
#endif

    bool m_is_high_q_quit;
    bool m_is_low_q_quit;
    bool m_needle_deserial_finish;
    bool m_speed_deserial_finish;
    bool m_blink_deserial_finish;
    bool m_tc_deserial_finish;
};


class DeserializeDash
{
public:
    DeserializeDash() noexcept {}
    egt::shared_cairo_surface_t DeSerialize(const std::string& filename, std::shared_ptr<egt::Rect>& rect);
    ~DeserializeDash() {}
};

#ifdef DO_SVG_SERIALIZATION
void MotorDash::SerializeSVG(const std::string& filename, egt::SvgImage& svg, const std::string& id)
{
    auto box = svg.id_box(id);
    auto layer = std::make_shared<egt::Image>(svg.render(id, box));

    egt::detail::ErawImage e;
    const auto data = cairo_image_surface_get_data(layer->surface().get());
    const auto width = cairo_image_surface_get_width(layer->surface().get());
    const auto height = cairo_image_surface_get_height(layer->surface().get());
    e.save(solve_relative_path(filename), data, box.x(), box.y(), width, height);
}

void MotorDash::SerializePNG(const char* png_src, const std::string& png_dst)
{
    egt::shared_cairo_surface_t surface;
    egt::detail::ErawImage e;
    surface =
            egt::shared_cairo_surface_t(cairo_image_surface_create_from_png(png_src),
                                        cairo_surface_destroy);
    const auto data = cairo_image_surface_get_data(surface.get());
    const auto width = cairo_image_surface_get_width(surface.get());
    const auto height = cairo_image_surface_get_height(surface.get());
    e.save(solve_relative_path(png_dst), data, 0, 0, width, height);
}

void MotorDash::serialize_all()
{
    int i;
    std::ostringstream str;
    std::ostringstream path;

    //Serialize scale PNG image
    SerializePNG("/usr/share/egt/examples/motorcycledash/moto.png", "eraw/moto_png.eraw");

    //Serialize background image
    SerializeSVG("eraw/bkgrd.eraw", m_svg, "#bkgrd");

    //Serialize Needles
    for (i = ID_MIN; i <= ID_MAX; i += STEPPER)
    {
        str.str("");
        path.str("");
        str << "#path" << std::to_string(i);
        path << "eraw/path" << std::to_string(i) << ".eraw";
        SerializeSVG(path.str(), m_svg, str.str());
    }

    //Serialize tc
    SerializeSVG("eraw/tc.eraw", m_svg, "#tc");

    //Serialize mute
    SerializeSVG("eraw/mute.eraw", m_svg, "#mute");

    //Serialize takeoff
    SerializeSVG("eraw/takeoff.eraw", m_svg, "#takeoff");

    //Serialize left_blink
    SerializeSVG("eraw/left_blink.eraw", m_svg, "#left_blink");

    //Serialize right_blink
    SerializeSVG("eraw/right_blink.eraw", m_svg, "#right_blink");

    //Serialize farlight
    SerializeSVG("eraw/farlight.eraw", m_svg, "#farlight");

    //Serialize mainspeed
    SerializeSVG("eraw/mainspeed.eraw", m_svg, "#mainspeed");

    //Serialize hazards
    SerializeSVG("eraw/hazards.eraw", m_svg, "#hazards");

    //Serialize abs
    SerializeSVG("eraw/abs.eraw", m_svg, "#abs");

    //Create a finish indicator
    if (-1 == system("touch /root/serialize_done"))
    {
        std::cout << "touch /root/serialize_done failed, please check permission!!!" << std::endl;
        return;
    }
    if (-1 == system("sync"))
    {
        std::cout << "sync failed, please check permission!!!" << std::endl;
        return;
    }
}
#endif

egt::shared_cairo_surface_t DeserializeDash::DeSerialize(const std::string& filename, std::shared_ptr<egt::Rect>& rect)
{
    return egt::detail::ErawImage::load(filename, rect);
}

egt::shared_cairo_surface_t MotorDash::DeSerialize(const std::string& filename, std::shared_ptr<egt::Rect>& rect)
{
    return egt::detail::ErawImage::load(solve_relative_path(filename), rect);
}

//When drawing in Inkscape, the y is starting from bottom but not from top,
//so we need convert this coordinate to EGT using, the y is starting from top
void MotorDash::ConvertInkscapeRect2EGT(std::shared_ptr<egt::Rect>& rect)
{
    rect->y(screen()->size().height() - rect->y() - rect->height());
}

//using QueueCallback = std::function<void ()>;
using QueueCallback = std::function<std::string ()>;

int main(int argc, char** argv)
{
    std::cout << "EGT start" << std::endl;
    std::queue<QueueCallback> high_pri_q;
    std::queue<QueueCallback> low_pri_q;

    //Widget handler
    std::vector<std::shared_ptr<egt::experimental::GaugeLayer>> NeedleBase;
    std::shared_ptr<egt::experimental::GaugeLayer> mutePtr;
    std::shared_ptr<egt::experimental::GaugeLayer> takeoffPtr;
    std::shared_ptr<egt::experimental::GaugeLayer> left_blinkPtr;
    std::shared_ptr<egt::experimental::GaugeLayer> right_blinkPtr;
    std::shared_ptr<egt::experimental::GaugeLayer> farlightPtr;
    std::shared_ptr<egt::experimental::GaugeLayer> hazardsPtr;
    std::shared_ptr<egt::experimental::GaugeLayer> absPtr;

    auto rect = std::make_shared<egt::Rect>();
    std::ostringstream str;
    std::ostringstream path;

#ifdef DO_SVG_SERIALIZATION
    bool need_serialization = false;
#endif

#ifdef MOTOR_SCALE_SUPPORT
    //Scale effect variables
    float scale_factor = 0.01;
    bool is_scale_rev = false;
    bool is_scale_2_max = false;
    bool is_scale_finish = false;
#endif

    egt::Application app(argc, argv);  //This call will cost ~270ms on 9x60ek board
    egt::TopWindow window;
    window.color(egt::Palette::ColorId::bg, egt::Palette::black);

#ifdef EXAMPLEDATA
    egt::add_search_path(EXAMPLEDATA);
#endif

#ifdef DO_SVG_SERIALIZATION
    //Check if serialize indicator "/serialize_done" exist? If not, need serialization
    if (access("/root/serialize_done", F_OK))
    {
        if (-1 == system("rm -rf /root/eraw"))
        {
            std::cout << "rm -rf /root/eraw failed, please check permission!!!" << std::endl;
            return -1;
        }
        if (0 > mkdir("/root/eraw", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH))
        {
            std::cout << "Create serialization dir eraw failed, please check permission!!!" << std::endl;
            return -1;
        }
        else
            need_serialization = true;
    }

    //If need serialization, use serialize.svg to parse SVG parameters, else use a blank one
    std::string svgpath = need_serialization ? "file:serialize.svg" : "file:deserialize.svg";
    auto svg = std::make_unique<egt::SvgImage>(svgpath, egt::SizeF(window.content_area().width(), 0));
    MotorDash motordash(*svg);
#else
    MotorDash motordash;
#endif

    window.add(motordash);
    motordash.show();
    window.show();
    motordash.add_text_widget("#textinit", "0", egt::Rect(900, 600, 2, 2), 1);
    std::cout << "EGT show" << std::endl;

    //Lambda for de-serializing background and needles
    auto DeserialNeedles = [&]()
    {
        //Background image and needles should be de-serialized firstly before main() return
        motordash.add(std::make_shared<egt::experimental::GaugeLayer>(egt::Image(motordash.DeSerialize("eraw/bkgrd.eraw", rect))));
        for (auto i = ID_MIN, j =0; i <= ID_MAX; i += STEPPER, j++)
        {
            path.str("");
            path << "eraw/path" << std::to_string(i) << ".eraw";
            NeedleBase.push_back(std::make_shared<egt::experimental::GaugeLayer>(egt::Image(motordash.DeSerialize(path.str(), rect))));
            NeedleBase[j]->box(*rect);
            NeedleBase[j]->hide();
            motordash.add(NeedleBase[j]);
        }
        motordash.add_text_widget("#speed", "0", egt::Rect(298, 145, 200, 120), 180);
    };

#ifdef DO_SVG_SERIALIZATION
    if (need_serialization)
    {
        //If need serialization, show indicator for user on screen
        auto text = std::make_shared<egt::TextBox>("EGT is serializing, please wait...", egt::Rect(70, 190, 700, 200));
        text->border(0);
        text->font(egt::Font(50, egt::Font::Weight::normal));
        text->color(egt::Palette::ColorId::bg, egt::Palette::transparent);
        text->color(egt::Palette::ColorId::text, egt::Palette::red);
        window.add(text);
        app.event().step();
        motordash.serialize_all();
        text->clear();
        text->text("Serialize successfully, welcome!");
        app.event().step();
        sleep(1);
        text->hide();
    }
#endif

#ifdef MOTOR_SCALE_SUPPORT
    std::cout << "Have libplanes" << std::endl;
    //Create scale image to HEO overlay
    egt::Sprite scale_s(egt::Image(motordash.DeSerialize("eraw/moto_png.eraw", rect)), egt::Size(400, 240), 1, egt::Point(200, 120),
                           egt::PixelFormat::xrgb8888, egt::WindowHint::heo_overlay);
    egt::detail::KMSOverlay* scale_ovl = reinterpret_cast<egt::detail::KMSOverlay*>(scale_s.screen());
    plane_set_pan_size(scale_ovl->s(), 0, 0);
    motordash.add(scale_s);
    scale_s.show();
#endif

    int speed_index = 0;
    int needle_index = 0;

    bool is_needle_finish = false;
    std::string lbd_ret = "0";
    int timer_cnt;

	//use timer or screen click to debug? 1:use timer; 0:use click
    egt::PeriodicTimer timer(std::chrono::milliseconds(10));
    timer.on_timeout([&]()
    {
        //motordash.get_high_q_state/get_low_q_state are used to protect high q not be interrupted by other event re-enter
        //if the state is false, it means this queue has not execute finished yet.
        if (!motordash.get_high_q_state())
            return;
        if (!high_pri_q.empty())
        {
            //gettimeofday(&time1, NULL);
            motordash.set_high_q_state(false);
            lbd_ret = high_pri_q.front()();
            high_pri_q.pop();
            motordash.set_high_q_state(true);
        }
        else
        {
            if (!motordash.get_low_q_state())
                return;
            if (!low_pri_q.empty())
            {
                //gettimeofday(&time1, NULL);
                motordash.set_low_q_state(false);
                lbd_ret = low_pri_q.front()();
                low_pri_q.pop();
                motordash.set_low_q_state(true);
                return;
            }
        }

        //viariable to control the animation and frequency
        timer_cnt = (LOW_Q_TIME_THRESHHOLD <= timer_cnt) ? 0 : timer_cnt + 1;

        //needle move function implemented by lambda
        auto needle_move = [&]()
        {
            //needles
            for (int i=0; i<8; i++)
            {
                if (is_increasing)
                {
                    NeedleBase[needle_index]->show();
                }
                else
                {
                    NeedleBase[needle_index]->hide();
                }

                if (is_increasing && MAX_NEEDLE_INDEX == needle_index)
                {
                    is_increasing = false;
                    is_needle_finish = true;
                    return "needle_move";
                }
                else if (!is_increasing && 0 == needle_index)
                {
                    is_increasing = true;
                    is_needle_finish = true;
                    return "needle_move";
                }
                else
                {
                    needle_index = is_increasing ? needle_index + 1 : needle_index - 1;
                }
            }
            return "needle_move";
        };

        //make sure the needles run a circle finish, then can enter to low priority procedure
        if (is_needle_finish)
        {
            is_needle_finish = false;

            //deserialize speed
            if (!motordash.get_speed_deserial_state())
            {
                motordash.set_speed_deserial_state(true);
                return;
            }
            else
            {
                if (!(timer_cnt % 1))
                {
                    low_pri_q.push([&]()
                    {
                        speed_index = (5 <= speed_index) ? 0 : speed_index + 1;
                        motordash.find_text("#speed")->clear();
                        switch (speed_index)
                        {
                            case 0:
                                motordash.find_text("#speed")->text("15");
                                break;
                            case 1:
                                motordash.find_text("#speed")->text("28");
                                break;
                            case 2:
                                motordash.find_text("#speed")->text("40");
                                break;
                            case 3:
                                motordash.find_text("#speed")->text("69");
                                break;
                            case 4:
                                //motordash.find_text("#speed")->color(egt::Palette::ColorId::text, egt::Palette::red);
                                motordash.find_text("#speed")->text("88");
                                break;
                            default:
                                motordash.find_text("#speed")->text("0");
                                break;
                        }

                        return "speed_change";
                    });
                }
            }

            //deserialize blink
            if (!motordash.get_blink_deserial_state())
            {
                low_pri_q.push([&]()
                {
                    left_blinkPtr = std::make_shared<egt::experimental::GaugeLayer>(egt::Image(motordash.DeSerialize("eraw/left_blink.eraw", rect)));
                    left_blinkPtr->box(*rect);
                    motordash.add(left_blinkPtr);
                    right_blinkPtr = std::make_shared<egt::experimental::GaugeLayer>(egt::Image(motordash.DeSerialize("eraw/right_blink.eraw", rect)));
                    right_blinkPtr->box(*rect);
                    motordash.add(right_blinkPtr);
                    return "blink_deserial";
                });
                motordash.set_blink_deserial_state(true);
                return;
            }
            else
            {
                if (!(timer_cnt % 3))
                {
                    low_pri_q.push([&]()
                    {
                        //blink
                        if (left_blinkPtr->visible())
                            left_blinkPtr->hide();
                        else
                            left_blinkPtr->show();

                        if (right_blinkPtr->visible())
                            right_blinkPtr->hide();
                        else
                            right_blinkPtr->show();

                        return "left_right_blink";
                    });
                }
            }
        }
        else  //this branch exec the high priority event
        {
            high_pri_q.push(needle_move);
        }
    });

#ifdef MOTOR_SCALE_SUPPORT
    egt::PeriodicTimer scale_timer(std::chrono::milliseconds(16));
    scale_timer.on_timeout([&]()
    {
        //scale lambda function definition
        auto show_scale = [&]()
        {
            if (2.2 <= scale_factor)
            {
                is_scale_rev = true;
                is_scale_2_max = true;
#ifdef SCALE_TO_MAX_AND_STAY
                is_scale_rev = false;
                is_scale_finish = true;
                scale_s.hide();
                scale_timer.stop();
                DeserialNeedles();
                timer.start();
                return;
#endif
            }
            if (0.001 >= scale_factor)
            {
                is_scale_rev = false;
                is_scale_finish = true;
                scale_s.hide();
                scale_timer.stop();
                DeserialNeedles();
                timer.start();
                return;
            }
            plane_set_pos(scale_ovl->s(), PLANE_WIDTH_HF - plane_width(scale_ovl->s())*scale_factor/2, PLANE_HEIGHT_HF - plane_height(scale_ovl->s())*scale_factor/2);
            plane_set_scale(scale_ovl->s(), scale_factor);
            plane_apply(scale_ovl->s());
            scale_factor = is_scale_rev ? scale_factor - 0.08 : scale_factor + 0.08;
        };

        //scale one step in every time out
        if (!is_scale_finish)
        {
            show_scale();
            return;
        }
    });
    scale_timer.start();
#else
    DeserialNeedles();
    timer.start();
#endif

#ifdef ENABLE_CAMERA_SUPPORT

	// camera handling
	bool playing = false;
	//egt::Size size(320, 240);
	egt::Size size(640, 480);
    auto format = egt::detail::enum_from_string<egt::PixelFormat>("yuyv");
    auto dev("/dev/video0");
	
	egt::CameraWindow player(size, dev, format, egt::WindowHint::heo_overlay);
    player.move_to_center(window.center());
    window.add(player);
	
	const auto wscale = static_cast<float>(egt::Application::instance().screen()->size().width()) / size.width();
    const auto hscale = static_cast<float>(egt::Application::instance().screen()->size().height()) / size.height();

	player.device(dev);
	player.move(egt::Point(0, 0));
	player.scale(wscale, hscale);
	
	player.on_connect([&player, &dev](const std::string & devnode)
    {
			std::cout << devnode << "is connected: starting it" << std::endl;
    });

	player.on_disconnect([&player, &playing, dev](const std::string & devnode)
    {
        if (player.device() == devnode)
        {
            std::cout << devnode << "is disconnected: stoping it" << std::endl;
            player.stop();
			playing = false;
        }
    });

#ifdef USE_KEY_EVENT
	// handle window event
	window.on_event([&player, &playing, &timer](egt::Event & event)
	{
        std::cout << event.key().keycode;
		
		if (event.id() == egt::EventId::keyboard_down)
		{
            std::cout << " down";
			
			player.hide();
			player.stop();
			timer.start();
			playing = false;
		}
        else if (event.id() == egt::EventId::keyboard_up)
		{
            std::cout << " up";
			
			timer.stop();
			player.start();
			playing = true;
			player.show();
				
			/*
			if( playing )
			{
				player.hide();
				player.stop();
				timer.start();
				playing = false;
			}
			else 
			{
				timer.stop();
				player.start();
				playing = true;
				player.show();
			}
			*/
		}
        else if (event.id() == egt::EventId::keyboard_repeat)
            std::cout << " repeat";
		
		std::cout << std::endl;
	}, {egt::EventId::keyboard_down,
        egt::EventId::keyboard_up,
        egt::EventId::keyboard_repeat
    });
#endif

#ifdef USE_GPIO_EVENT
	// NOLINTNEXTLINE(android-cloexec-open)
    int fd = open(GPIO_SET_REVERSE, 0);
    if (fd < 0)
    {
        std::cerr << "failed to open " << GPIO_SET_REVERSE << std::endl;
        return 1;
    }

    // setup the input and edges
    gpioevent_request ereq{};
    ereq.lineoffset = GPIO_PIN_REVERSE;
    ereq.handleflags = GPIOHANDLE_REQUEST_INPUT;
    ereq.eventflags = GPIOEVENT_REQUEST_BOTH_EDGES;
    if (ioctl(fd, GPIO_GET_LINEEVENT_IOCTL, &ereq) < 0)
        std::cerr << "GPIO_GET_LINEEVENT_IOCTL failed" << std::endl;

    // read initial state and get it out of the way
    gpiohandle_data data{};
    if (ioctl(ereq.fd, GPIOHANDLE_GET_LINE_VALUES_IOCTL, &data) < 0)
        std::cerr << "GPIOHANDLE_GET_LINE_VALUES_IOCTL failed" << std::endl;

    // start a monitor and set the label text when the GPIO changes
    GpioEventMonitor monitor(ereq.fd, [](const gpioevent_data & event)
    {
		std::cout << "Event ID: " << event.id << std::endl;
        auto status = event.id == GPIOEVENT_EVENT_RISING_EDGE ? "off" : "on";
        std::cout << status << std::endl;
    });
#endif

#endif // end of ENABLE_CAMERA_SUPPORT

    return app.run();
}
