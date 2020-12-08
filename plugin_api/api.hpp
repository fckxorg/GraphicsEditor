/*============================================================================*/

#include <cstdint>
#include <string>
#include <unordered_map>

/*============================================================================*/

namespace PluginAPI {

    /*  Структура используется для передачи плагину доступа к вашему
     *  канвасу в момент применения плагина. Не храните в ней владеющий
     *  указатель на массив пикселей.
     */
    struct Canvas {
        uint8_t* pixels;

        size_t height;
        size_t width;
    };

    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    struct Position {
        int64_t x;
        int64_t y;
    };

    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    struct Property {

        enum TYPE { PRIMARY_COLOR, SECONDARY_COLOR, THICKNESS };
        enum DISPLAY_TYPE { COLOR_PICKER, SLIDER, INPUTBOX, CHECKBOX };

        DISPLAY_TYPE display_type;

        const char* label;

        union {
            int32_t int_value;
            double double_value;
            void* pointer_value;
        };
    };

    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    struct PluginInfo {
        uint16_t version;
        std::string name;
    };

    class Plugin {
    public:
    
        /* init() вызывается в момент подгрузки плагина,
         * в ней нужно добавить в properties все свойства,
         * которые в дальнейшем потребуются плагину для настройки */
        virtual bool init() = 0; 

        /* deinit() вызывается при закрытии редактора, чтобы плагин
         * освободил ресурсы в случае, если он захватил их в init() */
        virtual bool deinit() = 0;
      
        /* start_apply() вызывается, когда Canvas, с которым работает плагин,
         * получает событие -- нажатие кнопки мыши */
        virtual void start_apply(Canvas canvas, Position pos) = 0;
 
        /* apply() вызывается, когда Canvas, с которым работает плагин,
         * получает событие -- перемещение нажатой мыши */
        virtual void apply(Canvas canvas, Position pos) = 0;
 
        /* apply() вызывается, когда Canvas, с которым работает плагин,
         * получает событие -- отжатие кнопки мыши */
        virtual void stop_apply(Canvas canvas, Position pos) = 0;

        virtual ~Plugin() = default;

        using PropertyMap = std::unordered_map<Property::TYPE, Property>;

        PropertyMap properties;
    };

    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    /* Функция для создания в статической области памяти объекта плагина.
     * Удобно использовать для определения функции get()
     */
    template <typename SomePlugin>
    PluginAPI::Plugin* get_instance() {    
        static SomePlugin PLUGIN;
        return &PLUGIN;
    }

} // namespace PluginAPI

/*============================================================================*/
////////////////////////////////////////////////////////////////////////////////

/* Функция, которую следует определить разработчику плагина 
 * для доступа к объекту плагина */
extern "C" PluginAPI::Plugin* get_plugin();

////////////////////////////////////////////////////////////////////////////////
/*============================================================================*/
