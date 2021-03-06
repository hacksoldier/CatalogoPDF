//
//  PSPDFBookmarkParser.h
//  PSPDFKit
//
//  Copyright (c) 2012 Peter Steinberger. All rights reserved.
//

#import "PSPDFKitGlobal.h"

@class PSPDFBookmark, PSPDFDocument;

/**
    Manages bookmarks for a PSPDFDocument.
 
    There is no notion of "bookmarks" in a PDF.
    (PDF "bookmarks" are entries in the Table Of Contents; which are parsed in PSPDFKit 
     by the PSPDFOutlineParser class)
 
    Bookmarks are saved in <APP>/Library/PrivateDocuments/<DocumentUID>/bookmark.plist
 
    All calls are thread safe.
 */
@interface PSPDFBookmarkParser : NSObject {
    dispatch_queue_t _bookmarkQueue;  // used for synchronization of _bookmarks
    NSMutableArray *_bookmarks;
}

/// Designated initializer
- (id)initWithDocument:(PSPDFDocument *)document;

/// Contains bookmarks (PSPDFBookmark) for the document.
@property(copy) NSArray *bookmarks;

@property(ps_weak) PSPDFDocument *document;

/// Convenience methods. Will return NO if page is invalid or bookmark doesn't exist.
/// If you manually add bookmarks, you might need to call createToolbarAnimated to update.
- (BOOL)addBookmarkForPage:(NSUInteger)page;
- (BOOL)removeBookmarkForPage:(NSUInteger)page;

/// Returns the bookmark if page has a bookmark.
- (PSPDFBookmark *)bookmarkForPage:(NSUInteger)page;

@end


@interface PSPDFBookmarkParser (SubclassingHooks)

/// Defaults to document.cacheDirectory/document.uid
/// documemt.cacheDirectory can be changed w/o subclassing PSPDFBookmarkParser.
- (NSString *)cachePath;

/// Defaults to cachePath/bookmarks.plist
- (NSString *)bookmarkPath;

/// Read bookmarks out of the plist in bookmarkPath.
- (NSArray *)loadBookmarks;

/**
    Saves the bookmark into a plist file at bookmarkPath.
    Most likely isn't called on the main thread, but do not block.

    We go the tricky way to make a plist so it's human-readable.
    Note that PSPDFDocument confirms to NSCopying, so streaming into a keyed archiver would work too.
*/
- (void)saveBookmarks;

@end