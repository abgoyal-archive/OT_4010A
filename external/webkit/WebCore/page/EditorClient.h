

#ifndef EditorClient_h
#define EditorClient_h

#include "EditorInsertAction.h"
#include "PlatformString.h"
#include "TextAffinity.h"
#include <wtf/Forward.h>
#include <wtf/Vector.h>

#if PLATFORM(MAC)
class NSArray;
class NSData;
class NSString;
class NSURL;
#endif

namespace WebCore {

class CSSStyleDeclaration;
class EditCommand;
class Element;
class Frame;
class HTMLElement;
class KeyboardEvent;
class Node;
class Range;
class VisibleSelection;
class String;
class VisiblePosition;

struct GrammarDetail {
    int location;
    int length;
    Vector<String> guesses;
    String userDescription;
};

enum TextCheckingType {
    TextCheckingTypeSpelling    = 1 << 1,
    TextCheckingTypeGrammar     = 1 << 2,
    TextCheckingTypeLink        = 1 << 5,
    TextCheckingTypeQuote       = 1 << 6,
    TextCheckingTypeDash        = 1 << 7,
    TextCheckingTypeReplacement = 1 << 8,
    TextCheckingTypeCorrection  = 1 << 9
};

struct TextCheckingResult {
    TextCheckingType type;
    int location;
    int length;
    Vector<GrammarDetail> details;
    String replacement;
};
 
class EditorClient {
public:
    virtual ~EditorClient() {  }
    virtual void pageDestroyed() = 0;
    
    virtual bool shouldDeleteRange(Range*) = 0;
    virtual bool shouldShowDeleteInterface(HTMLElement*) = 0;
    virtual bool smartInsertDeleteEnabled() = 0; 
    virtual bool isSelectTrailingWhitespaceEnabled() = 0;
    virtual bool isContinuousSpellCheckingEnabled() = 0;
    virtual void toggleContinuousSpellChecking() = 0;
    virtual bool isGrammarCheckingEnabled() = 0;
    virtual void toggleGrammarChecking() = 0;
    virtual int spellCheckerDocumentTag() = 0;
    
    virtual bool isEditable() = 0;

    virtual bool shouldBeginEditing(Range*) = 0;
    virtual bool shouldEndEditing(Range*) = 0;
    virtual bool shouldInsertNode(Node*, Range*, EditorInsertAction) = 0;
    virtual bool shouldInsertText(const String&, Range*, EditorInsertAction) = 0;
    virtual bool shouldChangeSelectedRange(Range* fromRange, Range* toRange, EAffinity, bool stillSelecting) = 0;
    
    virtual bool shouldApplyStyle(CSSStyleDeclaration*, Range*) = 0;
//  virtual bool shouldChangeTypingStyle(CSSStyleDeclaration* fromStyle, CSSStyleDeclaration* toStyle) = 0;
//  virtual bool doCommandBySelector(SEL selector) = 0;
    virtual bool shouldMoveRangeAfterDelete(Range*, Range*) = 0;

    virtual void didBeginEditing() = 0;
    virtual void respondToChangedContents() = 0;
    virtual void respondToChangedSelection() = 0;
    virtual void didEndEditing() = 0;
    virtual void didWriteSelectionToPasteboard() = 0;
    virtual void didSetSelectionTypesForPasteboard() = 0;
//  virtual void didChangeTypingStyle:(NSNotification *)notification = 0;
//  virtual void didChangeSelection:(NSNotification *)notification = 0;
//  virtual NSUndoManager* undoManager:(WebView *)webView = 0;
    
    virtual void registerCommandForUndo(PassRefPtr<EditCommand>) = 0;
    virtual void registerCommandForRedo(PassRefPtr<EditCommand>) = 0;
    virtual void clearUndoRedoOperations() = 0;

    virtual bool canUndo() const = 0;
    virtual bool canRedo() const = 0;
    
    virtual void undo() = 0;
    virtual void redo() = 0;

    virtual void handleKeyboardEvent(KeyboardEvent*) = 0;
    virtual void handleInputMethodKeydown(KeyboardEvent*) = 0;
    
    virtual void textFieldDidBeginEditing(Element*) = 0;
    virtual void textFieldDidEndEditing(Element*) = 0;
    virtual void textDidChangeInTextField(Element*) = 0;
    virtual bool doTextFieldCommandFromEvent(Element*, KeyboardEvent*) = 0;
    virtual void textWillBeDeletedInTextField(Element*) = 0;
    virtual void textDidChangeInTextArea(Element*) = 0;

#if PLATFORM(MAC)
    virtual NSString* userVisibleString(NSURL*) = 0;
#ifdef BUILDING_ON_TIGER
    virtual NSArray* pasteboardTypesForSelection(Frame*) = 0;
#endif
#endif

#if PLATFORM(MAC) && !defined(BUILDING_ON_TIGER) && !defined(BUILDING_ON_LEOPARD)
    virtual void uppercaseWord() = 0;
    virtual void lowercaseWord() = 0;
    virtual void capitalizeWord() = 0;
    virtual void showSubstitutionsPanel(bool show) = 0;
    virtual bool substitutionsPanelIsShowing() = 0;
    virtual void toggleSmartInsertDelete() = 0;
    virtual bool isAutomaticQuoteSubstitutionEnabled() = 0;
    virtual void toggleAutomaticQuoteSubstitution() = 0;
    virtual bool isAutomaticLinkDetectionEnabled() = 0;
    virtual void toggleAutomaticLinkDetection() = 0;
    virtual bool isAutomaticDashSubstitutionEnabled() = 0;
    virtual void toggleAutomaticDashSubstitution() = 0;
    virtual bool isAutomaticTextReplacementEnabled() = 0;
    virtual void toggleAutomaticTextReplacement() = 0;
    virtual bool isAutomaticSpellingCorrectionEnabled() = 0;
    virtual void toggleAutomaticSpellingCorrection() = 0;
#endif

    virtual void ignoreWordInSpellDocument(const String&) = 0;
    virtual void learnWord(const String&) = 0;
    virtual void checkSpellingOfString(const UChar*, int length, int* misspellingLocation, int* misspellingLength) = 0;
    virtual String getAutoCorrectSuggestionForMisspelledWord(const String& misspelledWord) = 0;
    virtual void checkGrammarOfString(const UChar*, int length, Vector<GrammarDetail>&, int* badGrammarLocation, int* badGrammarLength) = 0;
#if PLATFORM(MAC) && !defined(BUILDING_ON_TIGER) && !defined(BUILDING_ON_LEOPARD)
    virtual void checkTextOfParagraph(const UChar* text, int length, uint64_t checkingTypes, Vector<TextCheckingResult>& results) = 0;
#endif
    virtual void updateSpellingUIWithGrammarString(const String&, const GrammarDetail& detail) = 0;
    virtual void updateSpellingUIWithMisspelledWord(const String&) = 0;
    virtual void showSpellingUI(bool show) = 0;
    virtual bool spellingUIIsShowing() = 0;
    virtual void getGuessesForWord(const String&, Vector<String>& guesses) = 0;
    virtual void setInputMethodState(bool enabled) = 0;
};

}

#endif // EditorClient_h

