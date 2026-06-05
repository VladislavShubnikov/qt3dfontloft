// The MIT License (MIT)
//
// Copyright (c) 2026 Vladislav Shubnikov
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
// contour.cpp
// Read 2d contour from json, normalize contour
//

#include <QCoreApplication> // NOLINT(clang-diagnostic-error)
#include <QFile>
#include <QDebug>
#include <QTextStream>

#include "geo.h"
#include "contour.h"
#include "json.hpp" // NOLINT(misc-include-cleaner)

namespace
{

bool samePoint(const geo::Point2f& pa, const geo::Point2f& pb)
{
    const float dx{ pb.x_ - pa.x_ };
    const float dy{ pb.y_ - pa.y_ };
    const float d2 = (dx * dx) + (dy * dy);
    constexpr float kSmall{ 1.0e-3F };
    return d2 < kSmall;
}

} // namespace

void geo::Contour::removeDuplicates()
{
    for (auto& vec : contours_)
    {
        auto n = static_cast<int>(vec.size());
        const geo::Point2f& p0 = vec[0];
        while (true)
        {
            const geo::Point2f& pLast = vec[n - 1];
            if (samePoint(p0, pLast))
            {
                vec.pop_back();
                n = static_cast<int>(vec.size());
                continue;
            }
            break;
        }
    }
}

// NOLINTNEXTLINE
void geo::Contour::scaleTo(float side)
{
    geo::Rect2f bbox; // NOLINT(misc-const-correctness)
    for (auto& vec : contours_)
    {
        for (const geo::Point2f& p : vec)
        {
            bbox.extend(p);
        }
    }
    const float w = bbox.width();
    const float h = bbox.height();
    const float maxSide = std::max(w, h);
    const float scale = side / maxSide;

    const float xCenter = (bbox.pMin_.x_ + bbox.pMax_.x_) * 0.5F;
    const float yCenter = (bbox.pMin_.y_ + bbox.pMax_.y_) * 0.5F;

    for (auto& vec : contours_)
    {
        for (geo::Point2f& p : vec)
        {
            const float x{ p.x_ - xCenter };
            const float y{ p.y_ - yCenter };
            
            const float xNew{x * scale};
            const float yNew{y * scale};
            p.x_ = xNew;
            p.y_ = yNew;
        }
    }


}

geo::ArrayContours2f geo::Contour::getContours() const
{
    return contours_;
}

// NOLINTNEXTLINE
bool geo::Contour::readCharacterFromJson(const QString& character)
{
    QString dir = QCoreApplication::applicationDirPath();
    QString pathToJson = dir + R"(/../../data/char_)" + character + ".json";
    if (!QFile::exists(pathToJson))
    {
        pathToJson = dir + R"(/../data/char_)" + character + ".json";
        if (!QFile::exists(pathToJson))
        {
            qDebug()  << "Cant find file: " << pathToJson << "\n";
            return false;
        }
    }

    QFile file(pathToJson);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug()  << "Cant open file: " << pathToJson << "\n";
        return false;
    }
    QTextStream inp(&file);

    // Read the entire file content into the string
    const QString fileText = inp.readAll();

    file.close();
    using json = nlohmann::json;
    using ArrContoursJson = std::vector<std::vector<std::pair<float, float>>>;

    try
    {
        // Parse the string into a json object
        const QByteArray arr = fileText.toLocal8Bit();
        const char* rawJsonData = static_cast<const char*>(arr.constData());
        auto jsonContours = json::parse(rawJsonData).get<ArrContoursJson>();

        contours_.clear();

        for (const auto& vec : jsonContours)
        {
            std::vector<geo::Point2f> outVec;
            for (const auto& p : vec)
            {
                geo::Point2f pt{ p.first, p.second };
                outVec.push_back(pt);
            }
            contours_.push_back(outVec);
        }
        return true;
    }
    catch (json::parse_error& e)
    {
        qDebug() << "Error parse json: " << e.what() << "\n";
        return false;
    }
}