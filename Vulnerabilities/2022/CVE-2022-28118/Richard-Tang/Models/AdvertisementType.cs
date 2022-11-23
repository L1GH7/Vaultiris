using Datory.Annotations;
using Newtonsoft.Json;
using Newtonsoft.Json.Converters;

namespace SSCMS.Advertisement.Models
{
    [JsonConverter(typeof(StringEnumConverter))]
    public enum AdvertisementType
    {
        [DataEnum(DisplayName = "漂浮广告")] FloatImage,
        [DataEnum(DisplayName = "全屏下推")] ScreenDown,
        [DataEnum(DisplayName = "弹出窗口")] OpenWindow,
    }
}
