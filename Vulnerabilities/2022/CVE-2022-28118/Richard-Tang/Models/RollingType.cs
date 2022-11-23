using Datory.Annotations;
using Newtonsoft.Json;
using Newtonsoft.Json.Converters;

namespace SSCMS.Advertisement.Models
{
    [JsonConverter(typeof(StringEnumConverter))]
    public enum RollingType
	{
        [DataEnum(DisplayName = "静止不动")] Static,
        [DataEnum(DisplayName = "跟随窗体滚动")] FollowingScreen,
        [DataEnum(DisplayName = "在窗体中不断移动")] FloatingInWindow,
    }
}
