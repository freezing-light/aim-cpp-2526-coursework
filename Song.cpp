/**
 * @file Song.cpp
 * @brief Implementation file for Song.
 */

#include "Song.h"

#include <algorithm>
#include <cctype>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace {

std::string trim_copy(const std::string &s) {
    auto first = std::find_if_not(
        s.begin(), s.end(), [](const unsigned char ch) { return std::isspace(ch); });
    auto last = std::find_if_not(
        s.rbegin(), s.rend(), [](const unsigned char ch) { return std::isspace(ch); }).base();
    if (first >= last) {
        return {};
    }
    return {first, last};
}

std::string to_lower_copy(const std::string &s) {
    std::string result = s;
    std::transform(result.begin(), result.end(), result.begin(),
                   [](const unsigned char ch) { return std::tolower(ch); });
    return result;
}

std::string join_tags(const std::vector<std::string> &tags) {
    if (tags.empty()) {
        return "";
    }
    std::ostringstream oss;
    for (size_t i = 0; i < tags.size(); ++i) {
        if (i > 0) {
            oss << ", ";
        }
        oss << tags[i];
    }
    return oss.str();
}

}

int Song::next_id_ = 1;

Song::Song(const std::string &title, const std::string &artist, int duration_sec, int rating) {
    std::string cleanTitle = trim_copy(title);
    std::string cleanArtist = trim_copy(artist);

    if (cleanTitle.empty()) {
        std::cout << "[错误] 标题不能为空\n";
        return;
    }
    if (cleanArtist.empty()) {
        std::cout << "[错误] 艺人不能为空\n";
        return;
    }
    if (duration_sec <= 0) {
        std::cout << "[错误] 时长必须为正整数（秒）\n";
        return;
    }
    if (rating < 1 || rating > 5) {
        std::cout << "[错误] 评分必须在 1...5 之间\n";
        return;
    }

    id_ = next_id_++;
    title_ = cleanTitle;
    artist_ = cleanArtist;
    duration_sec_ = duration_sec;
    rating_ = rating;
    valid_ = true;
}

bool Song::set_title(const std::string &t) {
    std::string cleanTitle = trim_copy(t);
    if (cleanTitle.empty()) {
        std::cout << "[提示] 标题不能为空，已忽略本次修改\n";
        return false;
    }
    title_ = cleanTitle;
    return true;
}

bool Song::set_artist(const std::string &a) {
    std::string cleanArtist = trim_copy(a);
    if (cleanArtist.empty()) {
        std::cout << "[提示] 艺人不能为空，已忽略本次修改\n";
        return false;
    }
    artist_ = cleanArtist;
    return true;
}

bool Song::set_duration(int sec) {
    if (sec <= 0) {
        std::cout << "[提示] 时长需为正整数，已忽略本次修改\n";
        return false;
    }
    duration_sec_ = sec;
    return true;
}

bool Song::set_rating(int r) {
    if (r < 1 || r > 5) {
        std::cout << "[提示] 评分需在 1..5，已忽略本次修改\n";
        return false;
    }
    rating_ = r;
    return true;
}

bool Song::add_tag(const std::string &tag) {
    std::string cleanTag = trim_copy(tag);
    if (cleanTag.empty()) {
        std::cout << "[提示] 空标签已忽略\n";
        return false;
    }

    std::string lowerTag = to_lower_copy(cleanTag);
    for (const auto &existingTag : tags_) {
        if (to_lower_copy(existingTag) == lowerTag) {
            std::cout << "[提示] 标签已存在（忽略大小写）\n";
            return false;
        }
    }

    tags_.push_back(cleanTag);
    return true;
}

bool Song::remove_tag(const std::string &tag) {
    std::string lowerTag = to_lower_copy(trim_copy(tag));

    for (size_t i = 0; i < tags_.size(); ++i) {
        if (to_lower_copy(tags_[i]) == lowerTag) {
            tags_.erase(tags_.begin() + i);
            return true;
        }
    }

    std::cout << "[提示] 未找到该标签\n";
    return false;
}

bool Song::matches_keyword(const std::string &kw) const {
    std::string cleanKw = trim_copy(kw);
    if (cleanKw.empty()) {
        return false;
    }

    std::string lowerKw = to_lower_copy(cleanKw);
    std::string lowerTitle = to_lower_copy(title_);
    std::string lowerArtist = to_lower_copy(artist_);

    if (lowerTitle.find(lowerKw) != std::string::npos) {
        return true;
    }

    if (lowerArtist.find(lowerKw) != std::string::npos) {
        return true;
    }

    for (const auto &tag : tags_) {
        if (to_lower_copy(tag).find(lowerKw) != std::string::npos) {
            return true;
        }
    }

    return false;
}

std::ostream &operator<<(std::ostream &os, const Song &s) {
    os << "[#" << s.id_ << "] " << s.artist_ << " - " << s.title_ << " (" << s.duration_sec_
       << "s) " << std::string(s.rating_, '*');

    if (!s.tags_.empty()) {
        os << "  [tags: " << join_tags(s.tags_) << "]";
    }

    return os;
}

bool operator<(const Song &a, const Song &b) {
    if (a.rating_ > b.rating_) {
        return true;
    }
    if (a.rating_ < b.rating_) {
        return false;
    }

    if (a.title_ < b.title_) {
        return true;
    }
    if (a.title_ > b.title_) {
        return false;
    }

    return a.id_ < b.id_;
}
