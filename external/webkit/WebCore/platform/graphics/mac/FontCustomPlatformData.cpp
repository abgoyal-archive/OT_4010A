

#include "config.h"
#include "FontCustomPlatformData.h"

#include <ApplicationServices/ApplicationServices.h>
#include "SharedBuffer.h"
#include "FontPlatformData.h"
#include "OpenTypeSanitizer.h"

namespace WebCore {

FontCustomPlatformData::~FontCustomPlatformData()
{
    if (m_atsContainer)
        ATSFontDeactivate(m_atsContainer, NULL, kATSOptionFlagsDefault);
    CGFontRelease(m_cgFont);
}

FontPlatformData FontCustomPlatformData::fontPlatformData(int size, bool bold, bool italic, FontRenderingMode)
{
    return FontPlatformData(m_cgFont, (ATSUFontID)m_atsFont, size, bold, italic);
}

FontCustomPlatformData* createFontCustomPlatformData(SharedBuffer* buffer)
{
    ASSERT_ARG(buffer, buffer);

#if ENABLE(OPENTYPE_SANITIZER)
    OpenTypeSanitizer sanitizer(buffer);
    RefPtr<SharedBuffer> transcodeBuffer = sanitizer.sanitize();
    if (!transcodeBuffer)
        return 0; // validation failed.
    buffer = transcodeBuffer.get();
#endif

    ATSFontContainerRef containerRef = 0;
    ATSFontRef fontRef = 0;

    RetainPtr<CGFontRef> cgFontRef;

#if !defined(BUILDING_ON_TIGER) && !defined(BUILDING_ON_LEOPARD)
    RetainPtr<CFDataRef> bufferData(AdoptCF, buffer->createCFData());
    RetainPtr<CGDataProviderRef> dataProvider(AdoptCF, CGDataProviderCreateWithCFData(bufferData.get()));
    
    cgFontRef.adoptCF(CGFontCreateWithDataProvider(dataProvider.get()));
    if (!cgFontRef)
        return 0;
#else
    // Use ATS to activate the font.

    // The value "3" means that the font is private and can't be seen by anyone else.
    ATSFontActivateFromMemory((void*)buffer->data(), buffer->size(), 3, kATSFontFormatUnspecified, NULL, kATSOptionFlagsDefault, &containerRef);
    if (!containerRef)
        return 0;
    ItemCount fontCount;
    ATSFontFindFromContainer(containerRef, kATSOptionFlagsDefault, 0, NULL, &fontCount);
    
    // We just support the first font in the list.
    if (fontCount == 0) {
        ATSFontDeactivate(containerRef, NULL, kATSOptionFlagsDefault);
        return 0;
    }
    
    ATSFontFindFromContainer(containerRef, kATSOptionFlagsDefault, 1, &fontRef, NULL);
    if (!fontRef) {
        ATSFontDeactivate(containerRef, NULL, kATSOptionFlagsDefault);
        return 0;
    }
    
    cgFontRef.adoptCF(CGFontCreateWithPlatformFont(&fontRef));
#ifndef BUILDING_ON_TIGER
    // Workaround for <rdar://problem/5675504>.
    if (cgFontRef && !CGFontGetNumberOfGlyphs(cgFontRef.get()))
        cgFontRef = 0;
#endif
    if (!cgFontRef) {
        ATSFontDeactivate(containerRef, NULL, kATSOptionFlagsDefault);
        return 0;
    }
#endif // !defined(BUILDING_ON_TIGER) && !defined(BUILDING_ON_LEOPARD)

    return new FontCustomPlatformData(containerRef, fontRef, cgFontRef.releaseRef());
}

}
