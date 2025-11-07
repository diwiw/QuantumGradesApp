#include "Grades.hpp"
#include "common/LogLevel.hpp"
#include "utils/LoggerFactory.hpp"
#include "core/Statistics.hpp"
#include <iostream>
#include <numeric>
#include <algorithm>

namespace qga {

Grades::Grades() {
    logger_ = qga::utils::LoggerFactory::createLogger(
        "GradesDemo",
        "logs/grades_demo.log",
        qga::LogLevel::Info
    );
}

void Grades::add(int grade) {
    notes_.push_back(grade);
    logger_->log(qga::LogLevel::Info, fmt::format("[Grades] Added grade: {}", grade));
}

float Grades::getMean() const {
    if (notes_.empty()) {
        logger_->log(qga::LogLevel::Warn, "[Grades] Attempted to calculate mean with empty list");
        return 0.0f;
    }

    auto mean_opt = Statistics::calculateMean(notes_);
    if (mean_opt.has_value()) {
        logger_->log(qga::LogLevel::Info, fmt::format("[Grades] Calculated mean: {}", mean_opt.value()));
        return static_cast<float>(mean_opt.value());
    } else {
        logger_->log(qga::LogLevel::Err, "[Grades] Failed to calculate mean");
        return 0.0f;
    }
}

float Grades::getMedian() const {
    if (notes_.empty()) {
        logger_->log(qga::LogLevel::Warn, "[Grades] Attempted to calculate median with empty list");
        return 0.0f;
    }

    auto median_opt = Statistics::calculateMedian(notes_);
    if (median_opt.has_value()) {
        logger_->log(qga::LogLevel::Info, fmt::format("[Grades] Calculated median: {}", median_opt.value()));
        return static_cast<float>(median_opt.value());
    } else {
        logger_->log(qga::LogLevel::Err, "[Grades] Failed to calculate median");
        return 0.0f;
    }
}

float Grades::getStdDev() const {
    if (notes_.size() < 2) {
        logger_->log(qga::LogLevel::Warn, "[Grades] Not enough grades to calculate stddev");
        return 0.0f;
    }

    auto stddev_opt = Statistics::calculateStdDev(notes_);
    if (stddev_opt.has_value()) {
        logger_->log(qga::LogLevel::Info, fmt::format("[Grades] Calculated stddev: {}", stddev_opt.value()));
        return static_cast<float>(stddev_opt.value());
    } else {
        logger_->log(qga::LogLevel::Err, "[Grades] Failed to calculate stddev");
        return 0.0f;
    }
}

void Grades::printGrades() const {
    for (int g : notes_)
        std::cout << g << " ";
    std::cout << "\n";
}

void Grades::printSummary() const {
    if (notes_.empty()) {
        std::cout << "No grades available.\n";
        return;
    }

    std::cout << "\n=== Statistics ===\n";
    auto mean   = qga::Statistics::calculateMean(notes_);
    auto median = qga::Statistics::calculateMedian(notes_);
    auto stddev = qga::Statistics::calculateStdDev(notes_);
    auto max_v  = qga::Statistics::calculateMax(notes_);
    auto min_v  = qga::Statistics::calculateMin(notes_);

    if (mean)   std::cout << "Mean: "   << mean.value()   << "\n";
    if (median) std::cout << "Median: " << median.value() << "\n";
    if (stddev) std::cout << "StdDev: " << stddev.value() << "\n";
    if (max_v)  std::cout << "Max: "    << max_v.value()  << "\n";
    if (min_v)  std::cout << "Min: "    << min_v.value()  << "\n";
}

const std::vector<int>& Grades::getNotes() const {
    return notes_;
}

int Grades::getMax() const {
    if (notes_.empty()) {
        logger_->log(qga::LogLevel::Warn, "[Grades] Attempted to calculate max with empty list");
        return 0;
    }

    auto max_opt = qga::Statistics::calculateMax(notes_);
    if (max_opt.has_value()) {
        logger_->log(qga::LogLevel::Info, fmt::format("[Grades] Max grade: {}", max_opt.value()));
        return max_opt.value();
    } else {
        logger_->log(qga::LogLevel::Err, "[Grades] Failed to calculate max");
        return 0;
    }
}

int Grades::getMin() const {
    if (notes_.empty()) {
        logger_->log(qga::LogLevel::Warn, "[Grades] Attempted to calculate min with empty list");
        return 0;
    }

    auto min_opt = qga::Statistics::calculateMin(notes_);
    if (min_opt.has_value()) {
        logger_->log(qga::LogLevel::Info, fmt::format("[Grades] Min grade: {}", min_opt.value()));
        return min_opt.value();
    } else {
        logger_->log(qga::LogLevel::Err, "[Grades] Failed to calculate min");
        return 0;
    }
}

} // namespace qga
