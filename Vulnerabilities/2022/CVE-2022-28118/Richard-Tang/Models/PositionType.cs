using Datory.Annotations;
using Newtonsoft.Json;
using Newtonsoft.Json.Converters;

namespace SSCMS.Advertisement.Models
{
    [JsonConverter(typeof(StringEnumConverter))]
    public enum PositionType
	{
        [DataEnum(DisplayName = "左上")] LeftTop,
        [DataEnum(DisplayName = "左下")] LeftBottom,
        [DataEnum(DisplayName = "右上")] RightTop,
        [DataEnum(DisplayName = "右下")] RightBottom,
	}
}
