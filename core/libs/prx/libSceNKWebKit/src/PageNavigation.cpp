#include "prx/libSceNKWebKit/src/BackForwardList.hpp"
#include "prx/libSceNKWebKit/src/Configuration.hpp"
#include "prx/libSceNKWebKit/include/WebKitValueExports.hpp"

namespace {

void UpdateNavigationState(NKWebKit::Page* page, std::size_t index) {
    page->backForwardList->currentIndex = index;
    const auto& entry = page->backForwardList->entries[index];
    if (entry && entry->url.has_value()) {
        page->activeURL = NKWebKit::Make<NKWebKit::URL>(*entry->url);
    } else {
        page->activeURL = nullptr;
    }
    page->canGoBack = (index > 0);
    page->canGoForward = (index + 1 < page->backForwardList->entries.size());
}

}

extern "C" {

bool APS5_VABI WKPageCanGoBack(const void* page) {
    const auto* p = NKWebKit::Cast<NKWebKit::Page>(page, __func__);
    if (p->backForwardList && p->backForwardList->currentIndex.has_value()) {
        return *p->backForwardList->currentIndex > 0;
    }
    return p->canGoBack;
}

bool APS5_VABI WKPageCanGoForward(const void* page) {
    const auto* p = NKWebKit::Cast<NKWebKit::Page>(page, __func__);
    if (p->backForwardList && p->backForwardList->currentIndex.has_value()) {
        return *p->backForwardList->currentIndex + 1 < p->backForwardList->entries.size();
    }
    return p->canGoForward;
}

void APS5_VABI WKPageGoBack(const void* page) {
    auto* p = NKWebKit::Cast<NKWebKit::Page>(page, __func__);
    if (p->backForwardList && p->backForwardList->currentIndex.has_value() && *p->backForwardList->currentIndex > 0) {
        UpdateNavigationState(p, *p->backForwardList->currentIndex - 1);
    } else {
        p->canGoForward = true;
    }
}

void APS5_VABI WKPageGoForward(const void* page) {
    auto* p = NKWebKit::Cast<NKWebKit::Page>(page, __func__);
    if (p->backForwardList && p->backForwardList->currentIndex.has_value() &&
        *p->backForwardList->currentIndex + 1 < p->backForwardList->entries.size()) {
        UpdateNavigationState(p, *p->backForwardList->currentIndex + 1);
    } else {
        p->canGoBack = true;
    }
}

void APS5_VABI WKPageGoToBackForwardListItem(const void* page, const void* item) {
    auto* p = NKWebKit::Cast<NKWebKit::Page>(page, __func__);
    const auto* target = NKWebKit::Cast<NKWebKit::BackForwardListItem>(item, __func__);
    if (p->backForwardList) {
        for (std::size_t i = 0; i < p->backForwardList->entries.size(); ++i) {
            if (p->backForwardList->entries[i].Get() == target) {
                UpdateNavigationState(p, i);
                break;
            }
        }
    }
}

void APS5_VABI WKPageStopLoading(const void* page) {
    auto* p = NKWebKit::Cast<NKWebKit::Page>(page, __func__);
    p->isLoading = false;
    p->provisionalURL = nullptr;
    p->estimatedProgress = 1.0;
}

void APS5_VABI WKPageTryClose(const void* page) {
    auto* p = NKWebKit::Cast<NKWebKit::Page>(page, __func__);
    p->isClosed = true;
}

}
