#include "UfbxModelSourceReader.h"
#include "UfbxModelSource.h"
#include "../ThirdParty/ufbx.h"
#include <memory>

namespace Hiwoong
{
    ModelSourceReadResult UfbxModelSourceReader::Read(
        const std::string& filePath
    ) const
    {


        ModelSourceReadResult result;

        ufbx_load_opts options{};
        options.target_axes = {
             UFBX_COORDINATE_AXIS_POSITIVE_X, // 오른쪽
             UFBX_COORDINATE_AXIS_NEGATIVE_Y, // 위쪽
             UFBX_COORDINATE_AXIS_NEGATIVE_Z  // 전진(+Z)의 반대
        };
        options.target_unit_meters = 1.0;
        options.space_conversion = UFBX_SPACE_CONVERSION_MODIFY_GEOMETRY;
        ufbx_error error{};

        ufbx_scene* scene = ufbx_load_file(
            filePath.c_str(),
            &options,
            &error
        );

        if (scene == nullptr)
        {
            char message[1024]{};
            ufbx_format_error(message, sizeof(message), &error);

            result.errorMessage = message;
            return result;
        }

        result.source = std::make_shared<UfbxModelSource>(scene);
        result.success = true;

        return result;
    }
}

