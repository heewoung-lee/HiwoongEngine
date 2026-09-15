#include "BigTextDisplayComponent.h"
#include "Component/SpriteRendererComponent.h"
#include "Render/Renderer.h"

#include <array>
#include <string>
#include <unordered_map>
#include <cassert>

namespace Hiwoong
{
    //AI가 만들어줌 'A' ~ 9까지의 도안
    namespace
    {
        using Glyph = std::array<std::string, 5>;

        const std::unordered_map<char, Glyph> glyphs{
            {'A', {" ### ", "#   #", "#####", "#   #", "#   #"}},
            {'B', {"#### ", "#   #", "#### ", "#   #", "#### "}},
            {'C', {" ####", "#    ", "#    ", "#    ", " ####"}},
            {'D', {"#### ", "#   #", "#   #", "#   #", "#### "}},
            {'E', {"#####", "#    ", "#### ", "#    ", "#####"}},
            {'F', {"#####", "#    ", "#### ", "#    ", "#    "}},
            {'G', {" ####", "#    ", "# ###", "#   #", " ####"}},
            {'H', {"#   #", "#   #", "#####", "#   #", "#   #"}},
            {'I', {"#####", "  #  ", "  #  ", "  #  ", "#####"}},
            {'J', {"#####", "   # ", "   # ", "#  # ", " ##  "}},
            {'K', {"#   #", "#  # ", "###  ", "#  # ", "#   #"}},
            {'L', {"#    ", "#    ", "#    ", "#    ", "#####"}},
            {'M', {"#   #", "## ##", "# # #", "#   #", "#   #"}},
            {'N', {"#   #", "##  #", "# # #", "#  ##", "#   #"}},
            {'O', {" ### ", "#   #", "#   #", "#   #", " ### "}},
            {'P', {"#### ", "#   #", "#### ", "#    ", "#    "}},
            {'Q', {" ### ", "#   #", "#   #", "#  # ", " ## #"}},
            {'R', {"#### ", "#   #", "#### ", "#  # ", "#   #"}},
            {'S', {" ####", "#    ", " ### ", "    #", "#### "}},
            {'T', {"#####", "  #  ", "  #  ", "  #  ", "  #  "}},
            {'U', {"#   #", "#   #", "#   #", "#   #", " ### "}},
            {'V', {"#   #", "#   #", "#   #", " # # ", "  #  "}},
            {'W', {"#   #", "#   #", "# # #", "## ##", "#   #"}},
            {'X', {"#   #", " # # ", "  #  ", " # # ", "#   #"}},
            {'Y', {"#   #", " # # ", "  #  ", "  #  ", "  #  "}},
            {'Z', {"#####", "   # ", "  #  ", " #   ", "#####"}},

            {'0', {" ### ", "#  ##", "# # #", "##  #", " ### "}},
            {'1', {"  #  ", " ##  ", "  #  ", "  #  ", " ### "}},
            {'2', {" ### ", "#   #", "   # ", "  #  ", "#####"}},
            {'3', {"#### ", "    #", " ### ", "    #", "#### "}},
            {'4', {"#   #", "#   #", "#####", "    #", "    #"}},
            {'5', {"#####", "#    ", "#### ", "    #", "#### "}},
            {'6', {" ### ", "#    ", "#### ", "#   #", " ### "}},
            {'7', {"#####", "    #", "   # ", "  #  ", "  #  "}},
            {'8', {" ### ", "#   #", " ### ", "#   #", " ### "}},
            {'9', {" ### ", "#   #", " ####", "    #", " ### "}},

            {' ', {"     ", "     ", "     ", "     ", "     "}}
        };
    }


    BigTextDisplayComponent::BigTextDisplayComponent(
        const std::string& text, int scale)
        : text(text), scale(scale)
    {
        assert(scale >= 1);
    }

    //즉 글자를 받아서 여기서 바인딩된 문자를 스케일로 키워서 이미지를 만들고, SpriteRenderer에 전달함.
    //그래서 반드시 스프라이트렌더러가 필요.
    //지금(9.14)일 에서는 RequireComponent를 만들 여유는 없으니깐 그냥 사용.
    void BigTextDisplayComponent::Start()
    {
        super::Start();
        const auto renderer = GetComponent<SpriteRendererComponent>();
        assert(renderer != nullptr);

        renderer->SetImage(BuildImage());

    }
    //필수 이거 구독에 담을 메서드임.
    void BigTextDisplayComponent::SetText(const std::string& newText)
    {
        if (text == newText) return;
        text = newText;

        // 시작 전에는 문자열만 저장 하고 시작할때 표시.
        if (HasStared() == false) return;

        const std::shared_ptr<SpriteRendererComponent> renderer = GetComponent<SpriteRendererComponent>();
        assert(renderer != nullptr);

        renderer->SetImage(BuildImage());
    }

    Vector2 BigTextDisplayComponent::GetTextSize() const
    {
        if (text.empty()) return Vector2::Zero;

        const Glyph& glyph = glyphs.at(' ');

        const int count = static_cast<int>(text.size());
        //열 계산
        const int glyphWidth = static_cast<int>(glyph.front().size());
        //행 계산
        const int glyphHeight = static_cast<int>(glyph.size());

        //예를들어 "ABC"라면 
        //[ A: 5칸 ] 빈칸 [ B: 5칸 ] 빈칸 [ C: 5칸 ] 이건 가로칸
        //이니깐 17칸
        // 세로칸은 문자고정에 스케일만높인값
        return Vector2
        (
            (glyphWidth * count + count-1) * scale,
            glyphHeight * scale
        );
    }

    std::string BigTextDisplayComponent::BuildImage() const
    {
        std::string image;
        const std::size_t height = glyphs.at(' ').size(); //높이를 구한다.

        for (std::size_t row = 0; row < height; ++row)
        {
            std::string expandedRow;

            for (std::size_t index = 0; index < text.size();++index)
            {
                const auto found = glyphs.find(text[index]);

                //문자를 찾으면 해당도안
                const Glyph& glyph = found != glyphs.end() ? found->second : glyphs.at(' ');

                // 도안의 각 칸을 가로로 scale배 늘린다.
                for (char cell : glyph[row])
                {
                    expandedRow.append(scale, cell);
                }

                // 문자사이에 간격을 띄우기 위의 계산을 마치면 문자끼리 붙응 ㄹ수가 있음.
                if (index + 1 < text.size())
                {
                    expandedRow.append(scale, ' ');
                }
            }

            for (int repeat = 0; repeat < scale;++repeat)
            {
                if (image.empty() == false)
                {
                    image += '\n';
                }
                image += expandedRow;
            }
        }
        return image;
    }

}