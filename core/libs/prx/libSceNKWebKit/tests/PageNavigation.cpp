#include "prx/libc/include/general/VabiMacros.hpp"
#include "prx/libSceNKWebKit/include/WebKitValueExports.hpp"
#include "prx/libSceNKWebKit/src/BackForwardList.hpp"
#include "prx/libSceNKWebKit/src/Configuration.hpp"

#include <cstdio>
#include <cstdlib>
#include <source_location>
#include <stdexcept>
#include <vector>

namespace {

void Require(bool condition, std::source_location location = std::source_location::current()) {
    if (!condition) {
        std::fprintf(stderr, "WKPage navigation check failed at line %u\n", location.line());
        std::abort();
    }
}

template<class Function>
void RequireInvalidArgument(Function function) {
    bool threw = false;
    try {
        function();
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    Require(threw);
}

}

int main() {
    auto page = NKWebKit::Make<NKWebKit::Page>();
    const void* pageRef = NKWebKit::ToRef(page);
    Require(!WKPageCanGoBack(pageRef));
    Require(!WKPageCanGoForward(pageRef));
    Require(WKPageIsPinnedToLeftSide(pageRef));
    Require(WKPageIsPinnedToRightSide(pageRef));
    Require(WKPageIsPinnedToTopSide(pageRef));
    Require(WKPageIsPinnedToBottomSide(pageRef));

    page->canGoBack = true;
    page->canGoForward = true;
    page->isPinnedToLeftSide = false;
    page->isPinnedToRightSide = false;
    page->isPinnedToTopSide = false;
    page->isPinnedToBottomSide = false;
    Require(WKPageCanGoBack(pageRef));
    Require(WKPageCanGoForward(pageRef));
    Require(!WKPageIsPinnedToLeftSide(pageRef));
    Require(!WKPageIsPinnedToRightSide(pageRef));
    Require(!WKPageIsPinnedToTopSide(pageRef));
    Require(!WKPageIsPinnedToBottomSide(pageRef));

    page->canGoForward = false;
    WKPageGoBack(pageRef);
    Require(page->canGoForward);

    page->canGoBack = false;
    WKPageGoForward(pageRef);
    Require(page->canGoBack);

    auto item0 = NKWebKit::Make<NKWebKit::BackForwardListItem>("https://example.com/0", NKWebKit::Retained<NKWebKit::String>{}, std::nullopt);
    auto item1 = NKWebKit::Make<NKWebKit::BackForwardListItem>("https://example.com/1", NKWebKit::Retained<NKWebKit::String>{}, std::nullopt);
    auto item2 = NKWebKit::Make<NKWebKit::BackForwardListItem>("https://example.com/2", NKWebKit::Retained<NKWebKit::String>{}, std::nullopt);
    std::vector<NKWebKit::Retained<NKWebKit::BackForwardListItem>> entries;
    entries.push_back(item0);
    entries.push_back(item1);
    entries.push_back(item2);
    page->backForwardList = NKWebKit::Make<NKWebKit::BackForwardList>(std::move(entries), std::optional<std::size_t>(1));

    Require(WKPageCanGoBack(pageRef));
    Require(WKPageCanGoForward(pageRef));

    WKPageGoBack(pageRef);
    Require(page->backForwardList->currentIndex.value() == 0);
    Require(page->activeURL != nullptr);
    Require(page->activeURL->text == "https://example.com/0");
    Require(!WKPageCanGoBack(pageRef));
    Require(WKPageCanGoForward(pageRef));

    WKPageGoForward(pageRef);
    Require(page->backForwardList->currentIndex.value() == 1);
    Require(page->activeURL != nullptr);
    Require(page->activeURL->text == "https://example.com/1");
    Require(WKPageCanGoBack(pageRef));
    Require(WKPageCanGoForward(pageRef));

    WKPageGoToBackForwardListItem(pageRef, NKWebKit::ToRef(item2));
    Require(page->backForwardList->currentIndex.value() == 2);
    Require(page->activeURL != nullptr);
    Require(page->activeURL->text == "https://example.com/2");
    Require(WKPageCanGoBack(pageRef));
    Require(!WKPageCanGoForward(pageRef));

    page->isLoading = true;
    page->provisionalURL = NKWebKit::Make<NKWebKit::URL>("https://example.com/loading");
    page->estimatedProgress = 0.5;
    WKPageStopLoading(pageRef);
    Require(!page->isLoading);
    Require(page->provisionalURL == nullptr);
    Require(page->estimatedProgress == 1.0);

    Require(!page->isClosed);
    WKPageTryClose(pageRef);
    Require(page->isClosed);

    auto bundle = NKWebKit::Make<NKWebKit::Bundle>();
    const void* itemRef = NKWebKit::ToRef(item0);

    RequireInvalidArgument([] { WKPageCanGoBack(nullptr); });
    RequireInvalidArgument([] { WKPageCanGoForward(nullptr); });
    RequireInvalidArgument([] { WKPageIsPinnedToLeftSide(nullptr); });
    RequireInvalidArgument([] { WKPageIsPinnedToRightSide(nullptr); });
    RequireInvalidArgument([] { WKPageIsPinnedToTopSide(nullptr); });
    RequireInvalidArgument([] { WKPageIsPinnedToBottomSide(nullptr); });
    RequireInvalidArgument([] { WKPageGoBack(nullptr); });
    RequireInvalidArgument([] { WKPageGoForward(nullptr); });
    RequireInvalidArgument([&] { WKPageGoToBackForwardListItem(nullptr, itemRef); });
    RequireInvalidArgument([&] { WKPageGoToBackForwardListItem(pageRef, nullptr); });
    RequireInvalidArgument([&] { WKPageGoToBackForwardListItem(bundle.Get(), itemRef); });
    RequireInvalidArgument([&] { WKPageGoToBackForwardListItem(pageRef, bundle.Get()); });
    RequireInvalidArgument([] { WKPageStopLoading(nullptr); });
    RequireInvalidArgument([] { WKPageTryClose(nullptr); });

    WKRelease(pageRef);
    return 0;
}
