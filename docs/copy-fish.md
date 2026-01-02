Directory structure:
└── a9t9-copyfish/
    ├── README.md
    ├── dialog.html
    ├── LICENSE
    ├── manifest-firefox.json
    ├── manifest.json
    ├── manifest_chrome.json
    ├── manifest_firefox.json
    ├── message-dialog-action-popup.html
    ├── message-dialog-special-page.html
    ├── message-dialog.html
    ├── ocrlocal.html
    ├── options.html
    ├── overlay.html
    ├── screencapture.html
    ├── _locales/
    │   └── en/
    │       └── messages.json
    ├── config/
    │   └── config.json
    ├── scripts/
    │   ├── chromereload.js
    │   ├── crossbrowser.js
    │   ├── genlib.js
    │   ├── init-cs.js
    │   ├── message-dialog.js
    │   ├── ocrlocal.js
    │   ├── overlay.js
    │   ├── screencapture.js
    │   ├── ._background.js
    │   └── ._options.js
    └── styles/
        ├── cs.css
        ├── desktop.screencapture.css
        ├── options.css
        ├── reset.css
        ├── screencapture.css
        └── sprite-action-white.css


Files Content:

================================================
FILE: README.md
================================================
# Download Copyfish

Copyfish runs in Chrome, Edge and Firefox:

- [Copyfish OCR for Chrome](https://chromewebstore.google.com/detail/copyfish-%F0%9F%90%9F-free-ocr-soft/eenjdnjldapjajjofmldgmkjaienebbj)

- [Copyfish OCR for Firefox](https://addons.mozilla.org/en-US/firefox/addon/copyfish-ocr-software/)

-  [Copyfish OCR for Edge](https://microsoftedge.microsoft.com/addons/detail/copyfish-free-ocr-softw/ankheondabfngkjomknppbpkjcdabdlg)

For more information see https://ocr.space/copyfish


# What is Copyfish?

Copy, paste and translate text from images or videos with this free OCR software Chrome extension

Do you need to copy and paste text from images or videos? If yes, then the Copyfish free OCR software is for you.

“Images” come in all kinds of forms: photographs, charts, diagrams, screenshots, PDF documents, comics, error messages, memes – and Youtube movies.

Common reasons to convert images to text are if you want to google it, store it, email it or translate it. Until now, your only option was to retype the text. 

# For language learners and Manga readers 

There are many translator addons available, but they only work with plain website text.
 Text inside images, in the form of tricky Javascript/AJAX or, especially,
 in the form of movie subtitles on Youtube or Youku is unreachable for them.
 But not for Copyfish. And if you want, Copyfish also translates the text for you right away.

 For the subtitle translation use case, Copyfish has a repeat feature.
 Mark the area of the subtitle once and then use the repeat button to grab the text from
 this area repeatable as you watch the movie. 


# For developers

Copyfish is published under an open-source (GPL) license. 
As OCR software, Copyfish uses the free [OCR API](https://ocr.space/ocrapi) on https://ocr.space/. 





================================================
FILE: dialog.html
================================================
<div class="ocrext-element ocrext-wrapper">
  <header class="ocrext-element ocrext-header">
	<span class="ocrext-element ocrext-title mdl-layout-title">
    <span>Copyfish</span>
    <a class="ocrext-closeToolbar-link icon-action-white icon-action-white-ic_close_white_24dp" title="Close"><svg xmlns="http://www.w3.org/2000/svg" width="24" height="24" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
  <line x1="18" y1="6" x2="6" y2="18"/>
  <line x1="6" y1="6" x2="18" y2="18"/>
</svg>
</a>
<a class="ocrext-settings-link icon-action-white icon-action-white-ic_settings_white_24dp" title="Settings">
  <svg xmlns="http://www.w3.org/2000/svg" fill="#fff" xmlns:xlink="http://www.w3.org/1999/xlink" version="1.1" id="Capa_1" x="0px" y="0px" viewBox="0 0 297 297" style="enable-background:new 0 0 297 297;" xml:space="preserve"><g><path d="M148.5,81.888c-36.73,0-66.612,29.882-66.612,66.612s29.882,66.612,66.612,66.612s66.612-29.882,66.612-66.612   S185.23,81.888,148.5,81.888z M148.5,194.927c-25.599,0-46.427-20.827-46.427-46.427s20.827-46.427,46.427-46.427   s46.427,20.827,46.427,46.427S174.099,194.927,148.5,194.927z"/>
  <path d="M148.5,113.428c-19.339,0-35.072,15.733-35.072,35.072s15.733,35.072,35.072,35.072s35.072-15.733,35.072-35.072   S167.839,113.428,148.5,113.428z M148.5,163.387c-8.208,0-14.887-6.679-14.887-14.887s6.679-14.887,14.887-14.887   s14.887,6.679,14.887,14.887S156.708,163.387,148.5,163.387z"/>
  <path d="M288.789,182.326l-18.21-10.514c1.454-7.658,2.188-15.469,2.188-23.313s-0.734-15.655-2.188-23.313l18.21-10.514   c2.318-1.338,4.009-3.542,4.702-6.129c0.693-2.585,0.33-5.34-1.008-7.658l-30.757-53.273c-2.787-4.826-8.955-6.48-13.787-3.694   l-18.234,10.526c-11.872-10.249-25.607-18.192-40.355-23.332V10.093C189.35,4.519,184.831,0,179.257,0h-61.514   c-5.574,0-10.093,4.519-10.093,10.093v21.022c-14.748,5.14-28.483,13.082-40.355,23.332L49.061,43.92   c-4.829-2.788-11.001-1.133-13.787,3.694L4.517,100.887c-1.338,2.318-1.701,5.073-1.008,7.658c0.693,2.586,2.384,4.79,4.702,6.129   l18.21,10.514c-1.454,7.658-2.188,15.469-2.188,23.313s0.734,15.655,2.188,23.313l-18.21,10.514   c-2.318,1.338-4.009,3.542-4.702,6.129c-0.693,2.585-0.33,5.34,1.008,7.658l30.757,53.273c2.787,4.827,8.957,6.482,13.787,3.694   l18.234-10.526c11.872,10.249,25.607,18.192,40.355,23.332v21.022c0,5.574,4.519,10.093,10.093,10.093h61.514   c5.574,0,10.093-4.519,10.093-10.093v-21.022c14.748-5.14,28.483-13.082,40.355-23.332l18.234,10.526   c4.83,2.787,11.001,1.132,13.787-3.694l30.757-53.273c1.338-2.318,1.701-5.073,1.008-7.658   C292.798,185.869,291.107,183.665,288.789,182.326z M249.345,122.645c2.147,8.404,3.236,17.103,3.236,25.855   s-1.089,17.45-3.236,25.855c-1.121,4.384,0.814,8.977,4.732,11.239l15.878,9.167l-20.664,35.792l-15.908-9.184   c-3.921-2.262-8.87-1.64-12.104,1.526c-12.514,12.244-27.984,21.187-44.735,25.863c-4.362,1.217-7.379,5.192-7.379,9.721v18.336   h-41.329v-18.336c0-4.529-3.017-8.504-7.379-9.721c-16.752-4.676-32.221-13.618-44.735-25.863   c-3.237-3.166-8.186-3.788-12.104-1.526l-15.908,9.184l-20.664-35.792l15.878-9.167c3.918-2.262,5.853-6.855,4.732-11.239   c-2.147-8.404-3.236-17.103-3.236-25.855s1.089-17.45,3.236-25.855c1.121-4.384-0.814-8.977-4.732-11.239l-15.878-9.167   l20.664-35.792l15.908,9.184c3.922,2.263,8.87,1.64,12.104-1.526c12.514-12.244,27.984-21.187,44.735-25.863   c4.362-1.217,7.379-5.192,7.379-9.721V20.185h41.329v18.336c0,4.529,3.017,8.504,7.379,9.721   c16.752,4.676,32.221,13.618,44.735,25.863c3.238,3.167,8.186,3.789,12.104,1.526l15.908-9.184l20.664,35.792l-15.878,9.167   C250.16,113.668,248.225,118.261,249.345,122.645z"/></g><g></g><g></g><g></g><g></g><g></g><g></g><g></g><g></g><g></g><g></g><g></g><g></g><g></g><g></g><g></g>
</svg></a>
    <a class="ocrext-open-tab-link icon-action-white icon-action-white-ic_settings_white_24dp" title="Open in tab">
      <svg xmlns="http://www.w3.org/2000/svg" fill="#fff" xmlns:xlink="http://www.w3.org/1999/xlink" version="1.1" id="Layer_1" x="0px" y="0px" viewBox="0 0 460.002 460.002" style="enable-background:new 0 0 460.002 460.002;" xml:space="preserve"><g><g><path d="M427.137,0.001H32.865C14.743,0.001,0,14.745,0,32.866v394.272c0,18.121,14.743,32.864,32.865,32.864h394.271    c18.122,0,32.865-14.743,32.865-32.864V32.866h0.001C460.002,14.745,445.259,0.001,427.137,0.001z M32.865,430.001L32.865,430.001    c-1.582,0.001-2.865-1.282-2.865-2.864V32.866c0-1.579,1.285-2.865,2.865-2.865h394.271c1.58,0,2.865,1.285,2.865,2.864V344.12    l-26.378-26.378l10.999-10.999c8.58-8.58,11.146-21.484,6.503-32.694c-4.643-11.21-15.583-18.519-27.716-18.519H286.037    c-16.569,0-30,13.431-30,30v107.372c0,12.134,7.309,23.073,18.519,27.716c3.711,1.537,7.61,2.285,11.474,2.285    c7.808,0,15.48-3.048,21.22-8.788l10.999-10.999l26.885,26.885H32.865z M430.002,427.124c0,1.582-1.282,2.864-2.864,2.865    l-39.578,0.013l-69.311-69.311l-32.212,32.211V285.53h107.372l-32.211,32.212l68.804,68.804V427.124z"/></g></g><g></g><g></g><g></g><g></g><g></g><g></g><g></g><g></g><g></g><g></g><g></g><g></g><g></g><g></g><g></g></svg></a>
  </span>
  </header>
  <div class="ocrext-element ocrext-content ocrext-clearfix">
    <div class="ocrext-element ocrext-canvas-container">
      <canvas id="ocrext-canOrig" class="ocrext-element" width="480" height="320" style="display: none;"></canvas>
      <canvas id="ocrext-can" class="ocrext-element" width=480 height=320></canvas>
      <div class="ocrext-element ocrext-textoverlay-container">
        <a class="ocrext-close-link" title="Close"></a>
      </div>
    </div>
    <div class="ocrext-element ocrext-ocr-output">
      <div class="ocrext-element ocrext-spinner-wrapper">
        <div class="ocrext-element mdl-spinner mdl-js-spinner ocrext-spinner"></div>
      </div>
      <p class="ocrext-element ocrext-status"></p>
      <div class="ocrext-element ocrext-grid ocrext-grid-message ocrext-clearfix">
        <div class="ocrext-element ocrext-cell ocrext-cell-small">
          <h5 class="ocrext-element ocrext-label ocrext-message">OCR Result<span></span></h5>
        </div>
        <div class="ocrext-element ocrext-cell ocrext-cell-large">
          <textarea class="ocrext-element ocrext-ocr-message ocrext-result">N/A</textarea>
        </div>
      </div>
      <div class="ocrext-element ocrext-grid ocrext-grid-translated ocrext-clearfix">
        <div class="ocrext-element ocrext-cell ocrext-cell-small">
          <h5 class="ocrext-element ocrext-label ocrext-translated">Translated<span></span></h5>
        </div>
        <div class="ocrext-element ocrext-cell ocrext-cell-large">
          <p class="ocrext-element ocrext-ocr-translated ocrext-result">N/A</p>
        </div>
      </div>

    </div>
    <div class="ocrext-element ocrext-footer mdl-grid">
      <div class="ocrext-element mdl-cell mdl-cell--3-col ocrext-quickselect-btn-container">
      </div>
      <!-- <div class="ocrext-element mdl-cell mdl-cell--1-col">
      </div> -->
      <div class="ocrext-element mdl-cell mdl-cell--9-col ocrext-btn-container">
        <button class="ocrext-element ocrext-ocr-sendocr ocrext-btn mdl-button mdl-js-button mdl-button--raised mdl-js-ripple-effect mdl-button--accent" title="Redo OCR">Redo OCR</button>
        <button class="ocrext-element ocrext-ocr-recapture ocrext-btn mdl-button mdl-js-button mdl-button--raised mdl-js-ripple-effect mdl-button--accent" title="Recapture">Recapture</button>
        <button class="ocrext-element ocrext-ocr-retranslate ocrext-btn mdl-button mdl-js-button mdl-button--raised mdl-js-ripple-effect mdl-button--accent" title="Re-Translate">Re-Translate</button>
        <button class="ocrext-element ocrext-ocr-copy ocrext-btn mdl-button mdl-js-button mdl-button--raised mdl-js-ripple-effect mdl-button--accent" title="Copy to clipboard">Copy to clipboard</button>
        <div class="translate-button" id="deepl_translate_button"><img src="" alt="open translate"></div>
        <div  id="popup_translate_button"><img src="" alt="open translate"></div>
      </div>
    </div>

  </div>
</div>



================================================
FILE: LICENSE
================================================
                    GNU GENERAL PUBLIC LICENSE
                       Version 2, June 1991

 Copyright (C) 1989, 1991 Free Software Foundation, Inc., <http://fsf.org/>
 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 Everyone is permitted to copy and distribute verbatim copies
 of this license document, but changing it is not allowed.

                            Preamble

  The licenses for most software are designed to take away your
freedom to share and change it.  By contrast, the GNU General Public
License is intended to guarantee your freedom to share and change free
software--to make sure the software is free for all its users.  This
General Public License applies to most of the Free Software
Foundation's software and to any other program whose authors commit to
using it.  (Some other Free Software Foundation software is covered by
the GNU Lesser General Public License instead.)  You can apply it to
your programs, too.

  When we speak of free software, we are referring to freedom, not
price.  Our General Public Licenses are designed to make sure that you
have the freedom to distribute copies of free software (and charge for
this service if you wish), that you receive source code or can get it
if you want it, that you can change the software or use pieces of it
in new free programs; and that you know you can do these things.

  To protect your rights, we need to make restrictions that forbid
anyone to deny you these rights or to ask you to surrender the rights.
These restrictions translate to certain responsibilities for you if you
distribute copies of the software, or if you modify it.

  For example, if you distribute copies of such a program, whether
gratis or for a fee, you must give the recipients all the rights that
you have.  You must make sure that they, too, receive or can get the
source code.  And you must show them these terms so they know their
rights.

  We protect your rights with two steps: (1) copyright the software, and
(2) offer you this license which gives you legal permission to copy,
distribute and/or modify the software.

  Also, for each author's protection and ours, we want to make certain
that everyone understands that there is no warranty for this free
software.  If the software is modified by someone else and passed on, we
want its recipients to know that what they have is not the original, so
that any problems introduced by others will not reflect on the original
authors' reputations.

  Finally, any free program is threatened constantly by software
patents.  We wish to avoid the danger that redistributors of a free
program will individually obtain patent licenses, in effect making the
program proprietary.  To prevent this, we have made it clear that any
patent must be licensed for everyone's free use or not licensed at all.

  The precise terms and conditions for copying, distribution and
modification follow.

                    GNU GENERAL PUBLIC LICENSE
   TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION

  0. This License applies to any program or other work which contains
a notice placed by the copyright holder saying it may be distributed
under the terms of this General Public License.  The "Program", below,
refers to any such program or work, and a "work based on the Program"
means either the Program or any derivative work under copyright law:
that is to say, a work containing the Program or a portion of it,
either verbatim or with modifications and/or translated into another
language.  (Hereinafter, translation is included without limitation in
the term "modification".)  Each licensee is addressed as "you".

Activities other than copying, distribution and modification are not
covered by this License; they are outside its scope.  The act of
running the Program is not restricted, and the output from the Program
is covered only if its contents constitute a work based on the
Program (independent of having been made by running the Program).
Whether that is true depends on what the Program does.

  1. You may copy and distribute verbatim copies of the Program's
source code as you receive it, in any medium, provided that you
conspicuously and appropriately publish on each copy an appropriate
copyright notice and disclaimer of warranty; keep intact all the
notices that refer to this License and to the absence of any warranty;
and give any other recipients of the Program a copy of this License
along with the Program.

You may charge a fee for the physical act of transferring a copy, and
you may at your option offer warranty protection in exchange for a fee.

  2. You may modify your copy or copies of the Program or any portion
of it, thus forming a work based on the Program, and copy and
distribute such modifications or work under the terms of Section 1
above, provided that you also meet all of these conditions:

    a) You must cause the modified files to carry prominent notices
    stating that you changed the files and the date of any change.

    b) You must cause any work that you distribute or publish, that in
    whole or in part contains or is derived from the Program or any
    part thereof, to be licensed as a whole at no charge to all third
    parties under the terms of this License.

    c) If the modified program normally reads commands interactively
    when run, you must cause it, when started running for such
    interactive use in the most ordinary way, to print or display an
    announcement including an appropriate copyright notice and a
    notice that there is no warranty (or else, saying that you provide
    a warranty) and that users may redistribute the program under
    these conditions, and telling the user how to view a copy of this
    License.  (Exception: if the Program itself is interactive but
    does not normally print such an announcement, your work based on
    the Program is not required to print an announcement.)

These requirements apply to the modified work as a whole.  If
identifiable sections of that work are not derived from the Program,
and can be reasonably considered independent and separate works in
themselves, then this License, and its terms, do not apply to those
sections when you distribute them as separate works.  But when you
distribute the same sections as part of a whole which is a work based
on the Program, the distribution of the whole must be on the terms of
this License, whose permissions for other licensees extend to the
entire whole, and thus to each and every part regardless of who wrote it.

Thus, it is not the intent of this section to claim rights or contest
your rights to work written entirely by you; rather, the intent is to
exercise the right to control the distribution of derivative or
collective works based on the Program.

In addition, mere aggregation of another work not based on the Program
with the Program (or with a work based on the Program) on a volume of
a storage or distribution medium does not bring the other work under
the scope of this License.

  3. You may copy and distribute the Program (or a work based on it,
under Section 2) in object code or executable form under the terms of
Sections 1 and 2 above provided that you also do one of the following:

    a) Accompany it with the complete corresponding machine-readable
    source code, which must be distributed under the terms of Sections
    1 and 2 above on a medium customarily used for software interchange; or,

    b) Accompany it with a written offer, valid for at least three
    years, to give any third party, for a charge no more than your
    cost of physically performing source distribution, a complete
    machine-readable copy of the corresponding source code, to be
    distributed under the terms of Sections 1 and 2 above on a medium
    customarily used for software interchange; or,

    c) Accompany it with the information you received as to the offer
    to distribute corresponding source code.  (This alternative is
    allowed only for noncommercial distribution and only if you
    received the program in object code or executable form with such
    an offer, in accord with Subsection b above.)

The source code for a work means the preferred form of the work for
making modifications to it.  For an executable work, complete source
code means all the source code for all modules it contains, plus any
associated interface definition files, plus the scripts used to
control compilation and installation of the executable.  However, as a
special exception, the source code distributed need not include
anything that is normally distributed (in either source or binary
form) with the major components (compiler, kernel, and so on) of the
operating system on which the executable runs, unless that component
itself accompanies the executable.

If distribution of executable or object code is made by offering
access to copy from a designated place, then offering equivalent
access to copy the source code from the same place counts as
distribution of the source code, even though third parties are not
compelled to copy the source along with the object code.

  4. You may not copy, modify, sublicense, or distribute the Program
except as expressly provided under this License.  Any attempt
otherwise to copy, modify, sublicense or distribute the Program is
void, and will automatically terminate your rights under this License.
However, parties who have received copies, or rights, from you under
this License will not have their licenses terminated so long as such
parties remain in full compliance.

  5. You are not required to accept this License, since you have not
signed it.  However, nothing else grants you permission to modify or
distribute the Program or its derivative works.  These actions are
prohibited by law if you do not accept this License.  Therefore, by
modifying or distributing the Program (or any work based on the
Program), you indicate your acceptance of this License to do so, and
all its terms and conditions for copying, distributing or modifying
the Program or works based on it.

  6. Each time you redistribute the Program (or any work based on the
Program), the recipient automatically receives a license from the
original licensor to copy, distribute or modify the Program subject to
these terms and conditions.  You may not impose any further
restrictions on the recipients' exercise of the rights granted herein.
You are not responsible for enforcing compliance by third parties to
this License.

  7. If, as a consequence of a court judgment or allegation of patent
infringement or for any other reason (not limited to patent issues),
conditions are imposed on you (whether by court order, agreement or
otherwise) that contradict the conditions of this License, they do not
excuse you from the conditions of this License.  If you cannot
distribute so as to satisfy simultaneously your obligations under this
License and any other pertinent obligations, then as a consequence you
may not distribute the Program at all.  For example, if a patent
license would not permit royalty-free redistribution of the Program by
all those who receive copies directly or indirectly through you, then
the only way you could satisfy both it and this License would be to
refrain entirely from distribution of the Program.

If any portion of this section is held invalid or unenforceable under
any particular circumstance, the balance of the section is intended to
apply and the section as a whole is intended to apply in other
circumstances.

It is not the purpose of this section to induce you to infringe any
patents or other property right claims or to contest validity of any
such claims; this section has the sole purpose of protecting the
integrity of the free software distribution system, which is
implemented by public license practices.  Many people have made
generous contributions to the wide range of software distributed
through that system in reliance on consistent application of that
system; it is up to the author/donor to decide if he or she is willing
to distribute software through any other system and a licensee cannot
impose that choice.

This section is intended to make thoroughly clear what is believed to
be a consequence of the rest of this License.

  8. If the distribution and/or use of the Program is restricted in
certain countries either by patents or by copyrighted interfaces, the
original copyright holder who places the Program under this License
may add an explicit geographical distribution limitation excluding
those countries, so that distribution is permitted only in or among
countries not thus excluded.  In such case, this License incorporates
the limitation as if written in the body of this License.

  9. The Free Software Foundation may publish revised and/or new versions
of the General Public License from time to time.  Such new versions will
be similar in spirit to the present version, but may differ in detail to
address new problems or concerns.

Each version is given a distinguishing version number.  If the Program
specifies a version number of this License which applies to it and "any
later version", you have the option of following the terms and conditions
either of that version or of any later version published by the Free
Software Foundation.  If the Program does not specify a version number of
this License, you may choose any version ever published by the Free Software
Foundation.

  10. If you wish to incorporate parts of the Program into other free
programs whose distribution conditions are different, write to the author
to ask for permission.  For software which is copyrighted by the Free
Software Foundation, write to the Free Software Foundation; we sometimes
make exceptions for this.  Our decision will be guided by the two goals
of preserving the free status of all derivatives of our free software and
of promoting the sharing and reuse of software generally.

                            NO WARRANTY

  11. BECAUSE THE PROGRAM IS LICENSED FREE OF CHARGE, THERE IS NO WARRANTY
FOR THE PROGRAM, TO THE EXTENT PERMITTED BY APPLICABLE LAW.  EXCEPT WHEN
OTHERWISE STATED IN WRITING THE COPYRIGHT HOLDERS AND/OR OTHER PARTIES
PROVIDE THE PROGRAM "AS IS" WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED
OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  THE ENTIRE RISK AS
TO THE QUALITY AND PERFORMANCE OF THE PROGRAM IS WITH YOU.  SHOULD THE
PROGRAM PROVE DEFECTIVE, YOU ASSUME THE COST OF ALL NECESSARY SERVICING,
REPAIR OR CORRECTION.

  12. IN NO EVENT UNLESS REQUIRED BY APPLICABLE LAW OR AGREED TO IN WRITING
WILL ANY COPYRIGHT HOLDER, OR ANY OTHER PARTY WHO MAY MODIFY AND/OR
REDISTRIBUTE THE PROGRAM AS PERMITTED ABOVE, BE LIABLE TO YOU FOR DAMAGES,
INCLUDING ANY GENERAL, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES ARISING
OUT OF THE USE OR INABILITY TO USE THE PROGRAM (INCLUDING BUT NOT LIMITED
TO LOSS OF DATA OR DATA BEING RENDERED INACCURATE OR LOSSES SUSTAINED BY
YOU OR THIRD PARTIES OR A FAILURE OF THE PROGRAM TO OPERATE WITH ANY OTHER
PROGRAMS), EVEN IF SUCH HOLDER OR OTHER PARTY HAS BEEN ADVISED OF THE
POSSIBILITY OF SUCH DAMAGES.

                     END OF TERMS AND CONDITIONS

            How to Apply These Terms to Your New Programs

  If you develop a new program, and you want it to be of the greatest
possible use to the public, the best way to achieve this is to make it
free software which everyone can redistribute and change under these terms.

  To do so, attach the following notices to the program.  It is safest
to attach them to the start of each source file to most effectively
convey the exclusion of warranty; and each file should have at least
the "copyright" line and a pointer to where the full notice is found.

    {description}
    Copyright (C) {year}  {fullname}

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

Also add information on how to contact you by electronic and paper mail.

If the program is interactive, make it output a short notice like this
when it starts in an interactive mode:

    Gnomovision version 69, Copyright (C) year name of author
    Gnomovision comes with ABSOLUTELY NO WARRANTY; for details type `show w'.
    This is free software, and you are welcome to redistribute it
    under certain conditions; type `show c' for details.

The hypothetical commands `show w' and `show c' should show the appropriate
parts of the General Public License.  Of course, the commands you use may
be called something other than `show w' and `show c'; they could even be
mouse-clicks or menu items--whatever suits your program.

You should also get your employer (if you work as a programmer) or your
school, if any, to sign a "copyright disclaimer" for the program, if
necessary.  Here is a sample; alter the names:

  Yoyodyne, Inc., hereby disclaims all copyright interest in the program
  `Gnomovision' (which makes passes at compilers) written by James Hacker.

  {signature of Ty Coon}, 1 April 1989
  Ty Coon, President of Vice

This General Public License does not permit incorporating your program into
proprietary programs.  If your program is a subroutine library, you may
consider it more useful to permit linking proprietary applications with the
library.  If this is what you want to do, use the GNU Lesser General
Public License instead of this License.




================================================
FILE: manifest-firefox.json
================================================
{
  "background": {
    "scripts": [ "scripts/jquery.min.js", "scripts/chromereload.js", "scripts/background.js" ]
  },
  "applications": {
    "gecko": {
      "id": "copyfish@a9t9.com",
      "strict_min_version": "42.0"
    }
  },
  "version": "5.0.1",
  "browser_action": {
    "default_icon": {
      "19": "images/icon-19.png",
      "38": "images/icon-38.png"
    },
    "default_title": "__MSG_appName__"
  },
  "default_locale": "en",
  "description": "__MSG_appDescription__",
  "icons": {
    "128": "images/icon-128.png",
    "16": "images/icon-16.png",
    "19": "images/icon-19.png",
    "38": "images/icon-38.png"
  },
  "manifest_version": 2,
  "name": "__MSG_appName__",
  "permissions": [
    "contextMenus",
    "activeTab",
    "tabs",
    "<all_urls>",
    "storage",
    "notifications",
    "nativeMessaging"
  ],
  "options_ui": {
    "page": "options.html",
    "open_in_tab":true
  },
  "short_name": "__MSG_appShortName__",
  "web_accessible_resources": [ "dialog.html", "config/config.json", "images/gear.png", "images/outside.png", "images/close.png","images/translate.png"]
}



================================================
FILE: manifest.json
================================================
{
  "background": {
    "scripts": [ "scripts/jquery.min.js", "scripts/genlib.js", "scripts/chromereload.js", "scripts/background.js" ]
  },
  "browser_specific_settings": {
    "gecko": {
      "id": "copyfish@a9t9.com",
      "strict_min_version": "42.0"
    }
  },
  "version": "5.0.5",
  "browser_action": {
    "default_icon": {
      "16": "images/copyfish-16.png",
      "32": "images/copyfish-32.png",
      "48": "images/copyfish-48.png",
      "128": "images/copyfish-128.png"
    },
    "default_title": "__MSG_appName__"
  },
  "default_locale": "en",
  "description": "__MSG_appDescription__",
  "icons": {
      "16": "images/copyfish-16.png",
      "32": "images/copyfish-32.png",
      "48": "images/copyfish-48.png",
      "128": "images/copyfish-128.png"
  },
  "manifest_version": 2,
  "name": "__MSG_appName__",
  "permissions": [
    "contextMenus",
    "activeTab",
    "storage",
    "notifications",
    "nativeMessaging",
    "clipboardRead"
  ],
  "options_ui": {
    "page": "options.html",
    "open_in_tab":true
  },
  "short_name": "__MSG_appShortName__",
  "web_accessible_resources": [ "message-dialog.html","dialog.html", "config/config.json", "images/gear.png", "images/outside.png", "images/close.png","images/translate.png","images/deepl.jpg","images/copyfish-32.png"]
}



================================================
FILE: manifest_chrome.json
================================================
{
  "background": {
    "scripts": [ "scripts/jquery.min.js", "scripts/genlib.js", "scripts/chromereload.js", "scripts/background.js" ]
  },
  "version": "5.0.5",
  "browser_action": {
    "default_icon": {
      "16": "images/copyfish-16.png",
      "32": "images/copyfish-32.png",
      "48": "images/copyfish-48.png",
      "128": "images/copyfish-128.png"
    },
    "default_title": "__MSG_appName__"
  },
  "key": "MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAwiA5PGas1UzlJjOB/M0MlMgFbB3tgpsc65qsWSClriaNA83MZidgqtbi31GizfjWsTnhg1GvW9qLCi/HwQ/+fwSvskciGZc5qHdF9JPjc8C/S/MemoGbPNzV0Dw6+XJpz6Fw6OOK3O8Cr+SjPxnx73lWx3vT3ltQZq8D43X/s/1h/4nDo6WXv2GpynPrgdk8/Zx9qvLZueRRpcieygFvfjrL4ff5Hbpk2M/4ZlokP8rMtb205BmivyJ4mEutHTMds3R94nRyjfH+5jCSxoVPNsAHNbHLSoUwtpefCNRrVyIgcgJuPMfrIg4xs/BJxon9Uk2DopZSk4pprCeaZhR0PwIDAQAB",
  "default_locale": "en",
  "description": "__MSG_appDescription__",
  "icons": {
    "16": "images/copyfish-16.png",
    "32": "images/copyfish-32.png",
    "48": "images/copyfish-48.png",
    "128": "images/copyfish-128.png"
  },
  "manifest_version": 2,
  "name": "__MSG_appName__",
  "options_page": "options.html",
  "permissions": [
    "contextMenus",
    "activeTab",
    "storage",
    "notifications",
    "nativeMessaging"
  ],
  "short_name": "__MSG_appShortName__",
  "update_url": "https://clients2.google.com/service/update2/crx",
  "web_accessible_resources": [ "dialog.html", "config/config.json", "images/gear.png", "images/outside.png", "images/close.png","images/translate.png","images/deepl.jpg"]
}



================================================
FILE: manifest_firefox.json
================================================
{
  "content_scripts": [
    {
      "matches": ["<all_urls>"],
      "js": ["scripts/init-cs.js"],
      "match_about_blank": true,
      "run_at": "document_end"
    }],
  "background": {
    "scripts": [ "scripts/jquery.min.js", "scripts/genlib.js", "scripts/chromereload.js", "scripts/background.js" ]
  },
  "browser_specific_settings": {
    "gecko": {
      "id": "copyfish@a9t9.com",
      "strict_min_version": "42.0"
    }
  },
  "version": "5.0.5",
  "browser_action": {
    "default_icon": {
      "16": "images/copyfish-16.png",
      "32": "images/copyfish-32.png",
      "48": "images/copyfish-48.png",
      "128": "images/copyfish-128.png"
    },
    "default_title": "__MSG_appName__"
  },
  "default_locale": "en",
  "description": "__MSG_appDescription__",
  "icons": {
    "16": "images/copyfish-16.png",
    "32": "images/copyfish-32.png",
    "48": "images/copyfish-48.png",
    "128": "images/copyfish-128.png"
  },
  "manifest_version": 2,
  "name": "__MSG_appName__",
  "permissions": [
    "clipboardRead",
    "contextMenus",
    "activeTab",
    "tabs",
    "<all_urls>",
    "storage",
    "notifications",
    "nativeMessaging"
  ],
  "options_ui": {
    "page": "options.html",
    "open_in_tab":true
  },
  "short_name": "__MSG_appShortName__",
  "web_accessible_resources": [ "dialog.html", "config/config.json", "images/gear.png", "images/outside.png", "images/close.png","images/translate.png","images/deepl.jpg"]
}



================================================
FILE: message-dialog-action-popup.html
================================================
<!DOCTYPE html>
<html>

<head lang="en">
    <meta charset="UTF-8">
    <title>CopyFish</title>
    <!-- <script src="/scripts/jquery.min.js"></script> -->
    <!-- <script src="/js/popup.js"></script> -->
    <style>
        .popup-box {
            min-width: 400px;
            font-size: 13px;
            /* width: 30rem; */
            box-shadow: 0 0 1rem #ccc;
            min-height: 130px;
            /* margin-top:10px; */
            margin: 0px;
            /* padding: .5rem; */
            padding-top: 10px;
        }

        .cp-dialog-open-button {
            color: #FFF;
            background: #0066CC;
            padding: 10px;
            text-decoration: none;
            border: 1px solid #0157ad;
            border-radius: 3px;
        }
        
        /* Popup inner div */
        .cp-dialog-popup-content {
            /* width: 30%; */
            margin: 0 auto;
            box-sizing: border-box;
            padding: 40px;
            /* margin-top: 100px; */
            box-shadow: 0px 2px 6px rgba(0, 0, 0, 1);
            border-radius: 3px;
            background: #fff;
            /* position: relative; */
        }

        /* Popup close button */
       
        #cfish-popup-message-dialog {
            /* z-index: 999; */
            font-family: Roboto, Helvetica, Arial, sans-serif;
            color:#757575;
        }

        #cfish-popup-message-dialog .cp-dialog-popup-content {
            padding: 0;
        }

        #cfish-popup-message-dialog .title-con {
            display: flex;
            align-items: center;
            padding: 10px 15px;
            background: #2196f3;
        }

        #cfish-popup-message-dialog .title-con img {
            width: 25px;
            height: 25px;
            object-fit: cover;
        }

        #cfish-popup-message-dialog #cp-dialog-title {
            color: #fff;
            padding: 0;
            margin: 0 0 0 10px;
        }

        #cfish-popup-message-dialog #cp-dialog-description {
            padding: 10px 15px 20px;
            font-size: 18px;
            line-height: 25px;
        }

        #cfish-popup-message-dialog #cp-dialog-description .button-row {
            padding-top: 25px;
        }

        #cfish-popup-message-dialog #cp-dialog-description .button-row.btn-center {
            text-align: center;
        }

        #cfish-popup-message-dialog #cp-dialog-description span {
            display: inline-block;
            margin-right: 10px;
            margin-bottom: 10px;
            margin-top:12px;
        }

        #cfish-popup-message-dialog .cp-dialog-close-button {
            top: 7px;
            background: none;
            right: 5px;
        }

        .cp-dialog-description span {
            padding-top: 12px;
        }

        @media screen and (max-width: 720px) {
            .cp-dialog-popup-content {
                width: 90%;
            }
        }
    </style>
</head>

<body class="popup-box">
    <div class="cp-dialog-popup" popup-name="popup-1" id="cfish-popup-message-dialog">
        <div class="cp-dialog-popup-content">
            <div class="title-con">
                <img id='cp-dialog-image' src="/images/copyfish-48.png">
                <h2 id="cp-dialog-title">Copyfish</h2>
            </div>
            <p id="cp-dialog-description">
                <b>Copyfish</b> for Firefox can not access the clipboard on internal browser pages.
                <span>
                    <b>Solution:</b> Please load any normal website and try again.
                </span>
            </p>
        </div>
    </div>
</body>

</html>


================================================
FILE: message-dialog-special-page.html
================================================
<!DOCTYPE html>
<html>

<head lang="en">
    <meta charset="UTF-8">
    <title>CopyFish</title>
    <link crossorigin="anonymous" media="all" rel="stylesheet" href="/styles/material.min.css" />
    <style>
        .popup-box {
            min-width: 400px;
            font-size: 13px;
            box-shadow: 0 0 1rem #ccc;
            min-height: 130px;
            margin: 0px;
        }

        .cp-dialog-open-button {
            color: #FFF;
            background: #0066CC;
            padding: 10px;
            text-decoration: none;
            border: 1px solid #0157ad;
            border-radius: 3px;
        }

        /* Popup inner div */
        .cp-dialog-popup-content {
            /* width: 30%; */
            margin: 0 auto;
            box-sizing: border-box;
            padding: 40px;
            /* margin-top: 100px; */
            box-shadow: 0px 2px 6px rgba(0, 0, 0, 1);
            border-radius: 3px;
            background: #fff;
            /* position: relative; */
        }

        /* Popup close button */

        #cfish-popup-message-dialog {
            /* z-index: 999; */
            font-family: Roboto, Helvetica, Arial, sans-serif;
            color:#757575;
        }

        #cfish-popup-message-dialog .cp-dialog-popup-content {
            padding: 0;
        }

        #cfish-popup-message-dialog .title-con {
            display: flex;
            align-items: center;
            padding: 10px 15px;
            background:#2196f3;
        }

        #cfish-popup-message-dialog .title-con img {
            width: 25px;
            height: 25px;
            object-fit: cover;
        }

        #cfish-popup-message-dialog #cp-dialog-title {
            color: #fff;
            padding: 0;
            margin: 0 0 0 10px;
        }

        #cfish-popup-message-dialog #cp-dialog-description {
            padding: 10px 15px 20px;
            font-size: 18px;
            line-height: 25px;
        }

        #cfish-popup-message-dialog #cp-dialog-description .button-row {
            padding-top: 25px;
        }

        #cfish-popup-message-dialog #cp-dialog-description .button-row.btn-center {
            text-align: center;
        }

        #cfish-popup-message-dialog #cp-dialog-description span {
            display: inline-block;
            margin-right: 10px;
            margin-bottom: 10px;
            margin-top: 12px;
        }

        #cfish-popup-message-dialog .cp-dialog-close-button {
            top: 7px;
            background: none;
            right: 5px;
        }

        .cp-dialog-description span {
            padding-top: 12px;
        }

        @media screen and (max-width: 720px) {
            .cp-dialog-popup-content {
                width: 100%;
            }
        }
    </style>
    <script src='/scripts/jquery.min.js'></script>
    <script src='/scripts/message-dialog.js'></script>
</head>

<body class="popup-box">
    <div class="cp-dialog-popup" popup-name="popup-1" id="cfish-popup-message-dialog">
        <div class="cp-dialog-popup-content">
            <div class="title-con">
                <img id='cp-dialog-image' src="/images/copyfish-48.png">
                <h2 id="cp-dialog-title">Copyfish</h2>
            </div>
            <p id="cp-dialog-description">
                Welcome to the <b>Copyfish</b> extension.
                You can run this on any webpage, except
                for ones where the browser restricts
                extensions from running scripts. This
                includes the Chrome web store, browser
                settings pages, and other extensions.
                But you <i>can</i> use <a href='javascript:void(0);' id='readMoreLink' 
                >Desktop Screenshot</a> instead.
                This requires the installation of a helper app.
                <span>
                    <button id='downloadHelper'
                        class="cp-show-dialog-button ocrext-btn mdl-button mdl-js-button mdl-button--raised mdl-js-ripple-effect mdl-button--accent"
                        title="">
                        Download the helper app
                    </button>
                </span>
                <span>
                    <button id='downloadReadMore'
                        class="cp-show-dialog-button ocrext-btn mdl-button mdl-js-button mdl-button--raised mdl-js-ripple-effect mdl-button--accent"
                        title="">
                        Read More
                    </button>
                </span>
                <span>
                    <button id='cancelPopup'
                        class="cp-show-dialog-button ocrext-btn mdl-button mdl-js-button mdl-button--raised mdl-js-ripple-effect mdl-button--accent"
                        title="">
                        Close
                    </button>
                </span>
            </p>
        </div>
    </div>
</body>

</html>


================================================
FILE: message-dialog.html
================================================
<div class="cp-dialog-popup" popup-name="popup-1" id="cfish-popup-message-dialog">
    <div class="cp-dialog-popup-content">
        <div class="title-con">
            <img id='cp-dialog-image' src="">
            <h2 id="cp-dialog-title"></h2>
        </div>
        <p id="cp-dialog-description">
        </p>
        <a class="cp-dialog-close-button" popup-close="popup-1" href="javascript:void(0)">x</a>
    </div>
</div>


================================================
FILE: ocrlocal.html
================================================
﻿<!doctype html>
<html>

<head>
  <meta charset="utf-8">
  <link rel="stylesheet" href="styles/reset.css" />
  <link rel="stylesheet" href="styles/material.min.css" />
  <link href="styles/cs.css" rel="stylesheet">
  <link href="https://fonts.googleapis.com/css2?family=Roboto:wght@300;400;500;700&display=swap" rel="stylesheet">
  <!--tabs library-->
  <link href="styles/style-default.min.css" rel="stylesheet">
  <link href="styles/screencapture.css" rel="stylesheet">
  <link rel="stylesheet" href="styles/desktop.screencapture.css">
  <title>Tab View</title>
  <script src="scripts/jquery.min.js"></script>
  <script src="scripts/overlay.js"></script>
  <script src="scripts/cs.js"></script>
  <script src="scripts/material.min.js"></script>
 
</head>

<body>



  <div class="tabs-container">
    <div data-tabs class="data-tab-block">
      <div data-active>Copyfish</div>
      <div>Overlay</div>
      <div class="ocr-text-capture-tab">Text</div>
      <div class="translate-text-tab ocrext-grid-translated">Translation (PRO+)</div>
    </div>
    <div class="settings-icon-container">
      <img src="./images/gear-active.png" alt="settings" class="settings-icon ocrext-settings-link">
    </div>
  </div>


  <div data-panes id="screen-capture-overlay-tab"
    style="border-bottom:0px !important;width: 80%;box-sizing: border-box;overflow:auto;height: 100%;">
    <div id="copyfish-tab" class="tab-screenshot" style="height:100%">
      <div id="copyfish-tab-image-container" >
        <canvas id="ocrext-canOrig" class="ocrext-element" width="480" height="320" style="display: none;"></canvas>
        <canvas id="ocrext-can" class="ocrext-element" width=480 height=320></canvas>
        <div class="ocrext-element ocrext-textoverlay-container">
        </div>
      </div>
      <p class="ocrext-element ocrext-status"></p>
      <div class="copyfish-tab-text">
        <div class="copyfish-tab-text-lng">
          <h5 class="ocrext-element ocrext-label ocrext-message">OCR Result<br><span></span></h5>
        </div>
        <textarea name="capture-text" class="ocrext-element ocrext-ocr-message ocrext-result" cols="30"
          rows="10">N/A</textarea>
      </div>
      <div class="copyfish-tab-text copyfish-text-translate ocrext-grid-translated">
        <div class="copyfish-tab-text-lng">
          <h5 class="ocrext-element ocrext-label ocrext-translated">Translated<span></span></h5>
        </div>
        <div class="ocrext-element ocrext-cell ocrext-cell-large">
          <p class="ocrext-element ocrext-ocr-translated ocrext-result">N/A</p>
        </div>
      </div>
    </div>
    <div id="ocrext-element" class="tab-screenshot">
      <div class="ocrext-element ocrext-canvas-container overlay-tab">
        <img id="copyfish-tab-image" src="" />
        <div class="ocrext-element ocrext-textoverlay-container ocrext-container-only">
          <a class="ocrext-close-link" title="Close"></a>
        </div>
      </div>
    </div>
    <div class="text-capture-tab translate-text-tab tab-screenshot translated-textarea-tab ">
      <div><h5 class="ocrext-element ocrext-label ocrext-message">OCR Result<span></span>:</h5></div>
      <div class="translated-textarea">
      <textarea name="capture-text" class="ocrext-element ocrext-ocr-message ocrext-result" >N/A</textarea>
      </div>
    </div>
    <div class="translate-text-tab tab-screenshot translated-textarea-tab">
      <div class="ocrext-element ocrext-cell ocrext-cell-small">
        <h5 class="ocrext-element ocrext-label ocrext-translated">Translated<span></span>:</h5>
      </div>
      <div class="ocrext-element ocrext-cell ocrext-cell-large translated-textarea">
        <p class="ocrext-element ocrext-ocr-translated ocrext-result">N/A</p>
      </div>
    </div>
  </div>
  <div class="ocrext-element ocrext-footer mdl-grid button-screencapture">
    <!-- <div class="ocrext-element mdl-cell mdl-cell--2-col ocrext-quickselect-btn-container">
      </div> -->
    <!-- <div class="ocrext-element mdl-cell mdl-cell--1-col">
      </div> -->
    <div class="ocrext-element mdl-cell mdl-cell--12-col ocrext-btn-container align-flex-center">
      <button
        class="ocrext-element ocrext-ocr-sendocr ocrext-btn mdl-button mdl-js-button mdl-button--raised mdl-js-ripple-effect mdl-button--accent"
        title="Redo OCR">Redo OCR</button>
      <button
        class="ocrext-element ocrext-ocr-desktop-recapture ocrext-btn mdl-button mdl-js-button mdl-button--raised mdl-js-ripple-effect mdl-button--accent"
        title="Recapture">Recapture</button>
      <button
        class="ocrext-element ocrext-ocr-retranslate ocrext-btn mdl-button mdl-js-button mdl-button--raised mdl-js-ripple-effect mdl-button--accent"
        title="Re-Translate">Re-Translate</button>
      <button
        class="ocrext-element ocrext-ocr-copy ocrext-btn mdl-button mdl-js-button mdl-button--raised mdl-js-ripple-effect mdl-button--accent"
        title="Copy to clipboard">Copy to clipboard</button>
      <div id="deepl_translate_button" class="translate-button"><img src="./images/deepl.jpg" alt="open translate">
      </div>
      <div id="popup_translate_button"><img src="./images/translate.png" alt="open translate"></div>

    </div>
  </div>
  </div>

  <script src="scripts/tabbis.es6.min.js"></script>
  <script src="scripts/ocrlocal.js"></script>
</body>

</html>


================================================
FILE: options.html
================================================
<!doctype html>
<html lang="en">

<head>
  <meta charset="utf-8">
  <link rel="stylesheet" href="styles/material.min.css" />
  <link href="styles/options.css" rel="stylesheet">
  <link href="styles/cs.css" rel="stylesheet">
  <title>Options</title>
</head>

<body>
  <div class="container">
    <h1>Copyfish Options</h1>

    <div class="inner-wrapper">
      <div class="status-text">Settings saved successfully. Changes will be visible after the next conversion.</div>
      <span class="EnginesType">OCR Engine</span>
      <div class="OcrEngine">
        <tr>
          <td>
            <label class="mdl-radio mdl-js-radio  mdl-js-ripple-effect" for="OcrSpace">
              <input type="radio" id="OcrSpace" name="OCR" class="mdl-radio__button" value="OcrSpace">
              <span class="mdl-radio__label">OCR Engine 1
                (<a class="options-link" target="_blank" href="https://ocr.space/rd/copyfish?help=ocr1">more
                languages</a>)
              </span>
            </label>
          </td>

          <td>
            <label class="mdl-radio mdl-js-radio  mdl-js-ripple-effect" for="OcrSpaceSecond">
              <input type="radio" id="OcrSpaceSecond" name="OCR" class="mdl-radio__button" value="OcrSpaceSecond">
              <span class="mdl-radio__label">OCR Engine 2
                (better <a class="options-link" target="_blank"
                href="https://ocr.space/rd/copyfish?help=ocr2">alphanumeric OCR</a>)</span>
              </label>
            </td>

            <td>
              <label class="mdl-radio mdl-js-radio mdl-js-ripple-effect" for="OcrLocal">
                <input type="radio" id="OcrLocal" name="OCR" class="mdl-radio__button" value="OcrLocal">
                <span class="mdl-radio__label">XModule Local OCR (<a class="options-link" target="_blank"
                  href="https://ocr.space/rd/copyfish?help=xmodule-ocr">Install Link</a>)</span>
                </label>
              </td>

            </tr>
          </div>

            <div class="mdl-grid options-container input-language">
              <div class="mdl-cell mdl-cell--5-col label">
                <label>Input Language (OCR Language)</label>
              </div>
              <div class="mdl-cell mdl-cell--7-col">
                <select id="input-lang" name="inputlang">
                </select>
              </div>
            </div>
            <div class="mdl-grid options-container input-language-quickselect">
              <div class="mdl-cell mdl-cell--5-col label">
                <label>Input Language Quickselect</label>
              </div>
              <div class="mdl-cell mdl-cell--2-col">
                <select id="input-lang-1" name="inputlang" class="lang-quickselect">
                  <option value="none" selected="selected">None</option>
                </select>
              </div>
              <div class="mdl-cell mdl-cell--2-col">
                <select id="input-lang-2" name="inputlang" class="lang-quickselect">
                  <option value="none" selected="selected">None</option>
                </select>
              </div>
              <div class="mdl-cell mdl-cell--2-col">
                <select id="input-lang-3" name="inputlang" class="lang-quickselect">
                  <option value="none" selected="selected">None</option>
                </select>
              </div>
            </div>

  



    <div class="mdl-grid options-container font-size-options">
      <div class="mdl-cell mdl-cell--5-col label">
        <label>Text Box Font Size</label>
      </div>
      <div class="mdl-cell mdl-cell--7-col">
        <select id="ocr-fontsize" name="outputlang">
          <option value="48px">48 px</option>
          <option value="42px">42 px</option>
          <option value="36px">36 px</option>
          <option value="32px">32 px</option>
          <option value="28px">28 px</option>
          <option value="24px">24 px</option>
          <option value="20px">20 px</option>
          <option value="18px">18 px</option>
          <option value="16px" selected>16 px</option>
          <option value="14px">14 px</option>
          <option value="12px">12 px</option>
        </select>
      </div>
    </div>
    <div class="mdl-grid options-container translate-options">
      <div class="mdl-cell mdl-cell--5-col label">
        <label title="Support popup dictionaries">Support popup dictionaries <br />
          <a target="_blank" href="https://ocr.space/rd/copyfish?help=pdic" class="tip">Tip: How to use Copyfish with
          translator extensions</a>
        </label>
      </div>
      <div class="mdl-cell mdl-cell--7-col">
        <label class="mdl-switch mdl-js-switch mdl-js-ripple-effect popup-dicts" for="switch-popup-dicts">
          <input type="checkbox" id="switch-popup-dicts" class="mdl-switch__input" checked />
          <span class="mdl-switch__label"></span>
        </label>
      </div>
    </div>

    <div class="mdl-grid options-container translate-options">
      <div class="mdl-cell mdl-cell--5-col label">
        <label title="Use Table OCR">Use Table OCR</label>
      </div>
      <div class="mdl-cell mdl-cell--7-col">
        <label class="mdl-switch mdl-js-switch mdl-js-ripple-effect table-ocr" for="switch-table-ocr">
          <input type="checkbox" id="switch-table-ocr" class="mdl-switch__input" />
          <span class="mdl-switch__label"></span>
        </label>
      </div>
    </div>

    <div class="copy-auto-container">
      <h2>Copy text to clipboard</h2>
      <div class="mdl-grid options-container copy-auto-text">
        <div class="mdl-cell mdl-cell--5-col label">
          <label title="Copy automatically">Copy automatically <br />
          </label>
        </div>
        <div class="mdl-cell mdl-cell--7-col">
          <label class="mdl-switch mdl-js-switch mdl-js-ripple-effect copy-auto" for="switch-copy-auto">
            <input type="checkbox" id="switch-copy-auto" class="mdl-switch__input" checked />
            <span class="mdl-switch__label"></span>
          </label>
        </div>
      </div>

      </div>
      <div class="file-access-status">
        <h2>OCR Local Images and PDFs</h2>
        <div class="file-access-main">
          <p>With Copyfish you can also OCR local images and PDF. You only need to drag and drop them into your browser,
            and then use Copyfish on it. For this to work, you need to allow Copyfish access to file URLs. You do this
          in Chrome extension settings for Copyfish.</p>

          <div class="file-access-status-error">
            <span>Currently access to file URLs is not enabled. <a target="_blank"
              href="https://ocr.space/rd/copyfish?help=fileurl">Show me how to change this</a> </span>
            </div>

            <div class="file-access-status-done">
              <span>Access to file URLs is enabled.</span>
            </div>
          </div>
        </div>

        <div class="xmodule-item" id="xmodule-item-local">
          <div class="mdl-grid subtitle">
            <h2>LOCAL OCR Processing</h2>
          </div>

          <div class="file-access-main">
            <p>In order to  <a target="_blank"
                href="https://ocr.space/rd/copyfish?help=xmodule-ocr">use local OCR</a> you need to install the latest XModules. Local OCR is currently available for Windows and Mac, but <span
              style="color: red;">not</span> yet for Linux.</span>
            </p>
          </div>

          

          <div class="xmodule-title">
            <span><b>Status:</b> </span>

            <button id="check-update-fileaccess-local" type="button" class="ant-btn ant-btn-primary"><span>Test it</span>
            </button>

          </div>

          <div class="xmodule-status"><label>Status:&nbsp;</label>
            <div class="status-box local_fileaccess_module-span"><span style="color: red;">Not Installed</span>
              <a target="_blank" href="https://ocr.space/rd/copyfish?help=download">Download it</a>
              &nbsp;
              <span style="color: red;">Disabled</span>
            </div>
          </div>



        </div>
        <div class="xmodule-item" id="xmodule-item">
          <div class="mdl-grid subtitle">
            <h2>Desktop OCR</h2>
          </div>

          <div class="file-access-main">
            <p>Copyfish can grab text from anything on your the desktop with the help
              of a <b><span style="color: green;">free</span></b> additional local helper app. This app is called
              "XModule" (X like in in eXtension) and is available for Windows, Mac and Linux. The helper app is <span
              style="color: red;">not</span> required if you (only) want to use Copyfish with standard website content
              (<a href="https://ocr.space/rd/copyfish?help=desktop" target="_blank">More Info</a>).
            </br></br>
            Note: When Copyfish fails to grab text from a website it will try to use Desktop OCR instead. And if Desktop
            OCR
            is not installed, it will open this settings page. But again, if you do not need Desktop OCR there is no
            need to install the XModule app.

          </p>
        </div>

        <div class="mdl-grid mdl-cell--10-col options-container translate-options">
          <div class="mdl-cell label mdl-cell mdl-cell--6-col">
            <label title="Use Table OCR"><strong>Always use Desktop OCR</strong></label>
          </div>
          <div class="mdl-cell mdl-cell--5-col">
            <label class="mdl-switch mdl-js-switch mdl-js-ripple-effect usedesktop-ocr" for="switch-usedesktop-ocr">
              <input type="checkbox" id="switch-usedesktop-ocr" class="mdl-switch__input" />
              <span class="mdl-switch__label"></span>
            </label>
          </div>
        </div>

        <div class="xmodule-title">
          <span><b>File Access Status:</b> </span>

          <button id="check-update-fileaccess" type="button" class="ant-btn ant-btn-primary"><span>Test it</span>
          </button>

        </div>

        <div class="xmodule-status"><label>Status:&nbsp;</label>
          <div class="status-box fileaccess_module-span"><span style="color: red;">Not Installed</span>
            <a target="_blank" href="https://ocr.space/rd/copyfish?help=download">Download it</a>
            &nbsp;
            <span style="color: red;">Disabled</span>
          </div>
        </div>

        <div class="xmodule-title">
          <span><b>Desktop Screenshot Status:</b> </span>

          <button id="check-update-xmodule" type="button" class="ant-btn ant-btn-primary"><span>Test it</span>
          </button>

        </div>

        <div class="xmodule-status"><label>Status:&nbsp;</label>
          <div class="status-box xmodule-span"><span style="color: red;">Not Installed</span>
            <a target="_blank" href="https://ocr.space/rd/copyfish?help=download">Download it</a>
            &nbsp;
            <span style="color: red;">Disabled</span>
            <a href="#">Enable</a>
          </div>
          <div id="xmodule-shutter">
            Error: External Shutter program not found
          </div>
        </div>

      </div>
      <div class="mdl-grid subtitle">
        <h2>Global Keyboard Shortcuts</h2>
      </div>
      <p style="text-align: center;">Click <a href="#" id="manage-shortcuts">here</a> to change the shortcuts.</p>

     <div class="mdl-grid">
        <div class="mdl-cell mdl-cell--12-col">
        </div>
      </div>
	  
      <div class="mdl-grid">
        <div class="mdl-cell mdl-cell--12-col">
          <div class="subscription">
            <div class="text">🐟 Copyfish User Forum</div>
            <div><center>
                        <a target="_blank" href="https://ocr.space/rd/copyfish?help=forum">Meet developers and users in the OCR forum.</a>
            &nbsp;
		</center>
          </div>
        </div>
      </div>
	  
      <div class="mdl-grid">
        <div class="mdl-cell mdl-cell--12-col">
          <div class="subscription">
            <div class="text">👉 Need image/OCR-based browser automation?</div>
            <div><center>
                        <a target="_blank" href="https://ocr.space/rd/copyfish?help=getrpa">Get the Ui.Vision Extension for Chrome/Edge/Firefox.</br>It is Free, Open-Source & from the same developers as Copyfish!</a>
            &nbsp;
		</center>
          </div>
        </div>
      </div>

      <div class="mdl-grid options-container buttons">
        <!--   <div class="mdl-cell mdl-cell--3-col label"></div>
            <div class="mdl-cell mdl-cell--5-col button-cell">
             <button class="mdl-button mdl-js-button mdl-button--raised mdl-js-ripple-effect mdl-button--accent btn btn-reset" title="Reset">Reset</button> -->
             <!-- <button class="mdl-button mdl-js-button mdl-button--raised mdl-js-ripple-effect mdl-button--accent btn btn-save" title="Save">Save</button> -->
             <!--</div>-->
             <div class="mdl-cell mdl-cell--5-col label">
          
            </div>
          </div>
        </div>
      </div>

      <script src="scripts/jquery.min.js"></script>
      <script src="scripts/material.min.js"></script>
      <script src="scripts/genlib.js"></script>
      <script src="scripts/overlay.js"></script>
      <script src="scripts/options.js"></script>
    </body>

    </html>



================================================
FILE: overlay.html
================================================
﻿<!doctype html>
<html>
  <head>
    <meta charset="utf-8">
    <link rel="stylesheet" href="styles/material.min.css" />
    <link href="styles/cs.css" rel="stylesheet">
    <link href="styles/screencapture.css" rel="stylesheet">
    
    <title>Display Overlay</title>
  </head>
  <body class="ocrext-textoverlay-page" data-ocrext-run="listenToBackgroundEvents">
    <h1 class="ocrext-textoverlay-title"></h1>
    <div class="ocrext-textoverlay-container">
      <a class="ocrext-close-link" title="Close"></a>
      <!-- <div class="canvas-wrapper">
        <canvas id="main-canvas"></canvas>
      </div> -->
    </div>
    <script src="scripts/jquery.min.js"></script>
    <script src="scripts/material.min.js"></script>
    <script src="scripts/overlay.js"></script>
  </body>
</html>


================================================
FILE: screencapture.html
================================================
﻿<!doctype html>
<html>

<head>
  <meta charset="utf-8">
  <link rel="stylesheet" href="styles/reset.css" />
  <link rel="stylesheet" href="styles/material.min.css" />
  <link href="styles/cs.css" rel="stylesheet">
  <link href="https://fonts.googleapis.com/css2?family=Roboto:wght@300;400;500;700&display=swap" rel="stylesheet">
  <!--tabs library-->
  <link href="styles/style-default.min.css" rel="stylesheet">
  <link href="styles/screencapture.css" rel="stylesheet">
  <link rel="stylesheet" href="styles/desktop.screencapture.css">
  <title>Tab View</title>
  <script src="scripts/jquery.min.js"></script>
  <script src="scripts/overlay.js"></script>
  <script src="scripts/cs.js"></script>
  <script src="scripts/material.min.js"></script>
 
</head>

<body>



  <div class="tabs-container">
    <div data-tabs class="data-tab-block">
      <div data-active>Copyfish</div>
      <div>Overlay</div>
      <div class="ocr-text-capture-tab">Text</div>
    </div>
    <div class="settings-icon-container">
      <img src="./images/gear-active.png" alt="settings" class="settings-icon ocrext-settings-link">
    </div>
  </div>


  <div data-panes id="screen-capture-overlay-tab"
    style="border-bottom:0px !important;width: 80%;box-sizing: border-box;overflow:auto;height: 100%;">
    <div id="copyfish-tab" class="tab-screenshot" style="height:100%">
      <div id="copyfish-tab-image-container" >
        <canvas id="ocrext-canOrig" class="ocrext-element" width="480" height="320" style="display: none;"></canvas>
        <canvas id="ocrext-can" class="ocrext-element" width=480 height=320></canvas>
        <div class="ocrext-element ocrext-textoverlay-container">
        </div>
      </div>
      <p class="ocrext-element ocrext-status"></p>
      <div class="copyfish-tab-text">
        <div class="copyfish-tab-text-lng">
          <h5 class="ocrext-element ocrext-label ocrext-message">OCR Result<br><span></span></h5>
        </div>
        <textarea name="capture-text" class="ocrext-element ocrext-ocr-message ocrext-result" cols="30"
          rows="10">N/A</textarea>
      </div>
      <div class="copyfish-tab-text copyfish-text-translate ocrext-grid-translated">
        <div class="copyfish-tab-text-lng">
          <h5 class="ocrext-element ocrext-label ocrext-translated">Translated<span></span></h5>
        </div>
        <div class="ocrext-element ocrext-cell ocrext-cell-large">
          <p class="ocrext-element ocrext-ocr-translated ocrext-result">N/A</p>
        </div>
      </div>
    </div>
    <div id="ocrext-element" class="tab-screenshot">
      <div class="ocrext-element ocrext-canvas-container overlay-tab">
        <img id="copyfish-tab-image" src="" />
        <div class="ocrext-element ocrext-textoverlay-container ocrext-container-only">
          <a class="ocrext-close-link" title="Close"></a>
        </div>
      </div>
    </div>
    <div class="text-capture-tab translate-text-tab tab-screenshot translated-textarea-tab ">
      <div><h5 class="ocrext-element ocrext-label ocrext-message">OCR Result<span></span>:</h5></div>
      <div class="translated-textarea">
      <textarea name="capture-text" class="ocrext-element ocrext-ocr-message ocrext-result" >N/A</textarea>
      </div>
    </div>
    <div class="translate-text-tab tab-screenshot translated-textarea-tab">
      <div class="ocrext-element ocrext-cell ocrext-cell-small">
        <h5 class="ocrext-element ocrext-label ocrext-translated">Translated<span></span>:</h5>
      </div>
      <div class="ocrext-element ocrext-cell ocrext-cell-large translated-textarea">
        <p class="ocrext-element ocrext-ocr-translated ocrext-result">N/A</p>
      </div>
    </div>
  </div>
  <div class="ocrext-element ocrext-footer mdl-grid button-screencapture">
    <!-- <div class="ocrext-element mdl-cell mdl-cell--2-col ocrext-quickselect-btn-container">
      </div> -->
    <!-- <div class="ocrext-element mdl-cell mdl-cell--1-col">
      </div> -->
    <div class="ocrext-element mdl-cell mdl-cell--12-col ocrext-btn-container align-flex-center">
      <button
        class="ocrext-element ocrext-ocr-sendocr ocrext-btn mdl-button mdl-js-button mdl-button--raised mdl-js-ripple-effect mdl-button--accent"
        title="Redo OCR">Redo OCR</button>
      <button
        class="ocrext-element ocrext-ocr-desktop-recapture ocrext-btn mdl-button mdl-js-button mdl-button--raised mdl-js-ripple-effect mdl-button--accent"
        title="Recapture">Recapture</button>
      <button
        class="ocrext-element ocrext-ocr-retranslate ocrext-btn mdl-button mdl-js-button mdl-button--raised mdl-js-ripple-effect mdl-button--accent"
        title="Re-Translate">Re-Translate</button>
      <button
        class="ocrext-element ocrext-ocr-copy ocrext-btn mdl-button mdl-js-button mdl-button--raised mdl-js-ripple-effect mdl-button--accent"
        title="Copy to clipboard">Copy to clipboard</button>
      <div id="deepl_translate_button" class="translate-button"><img src="./images/deepl.jpg" alt="open translate">
      </div>
      <div id="popup_translate_button"><img src="./images/translate.png" alt="open translate"></div>

    </div>
  </div>
  </div>

  <script src="scripts/tabbis.es6.min.js"></script>
  <script src="scripts/screencapture.js"></script>
</body>

</html>


================================================
FILE: _locales/en/messages.json
================================================
{
   "appDescription": {
      "description": "The description of the application",
      "message": "Copy, paste and translate text from any image, video or PDF."
   },
   "appName": {
      "message": "Copyfish 🐟 Free OCR Software"
   },
   "appShortName": {
      "message": "Copyfish 🐟"
   },
   "captureError": {
      "description": "If page load is not yet done, throw an error",
      "message": "Unable to capture screen, please try again when is loaded. \nDo you want to capture the desktop instead?"
   },
   "captureFailure": {
      "description": "Could not capture media stream",
      "message": "Media capture failed"
   },
   "captureNotAvailable": {
      "description": "If this message does not disappear, try reloading the page.",
      "message": "If this message does not disappear, try reloading the page."
   },
   "nativeAppNotSupported": {
      "description": "The desktop screen capture helper is not yet installed. This free helper app is required as the browser extension alone can not take desktop screenshots.",
      "message": "The desktop screen capture helper is not yet installed. This free helper app is required as the browser extension alone can not take desktop screenshots."
   },
   "ocrDimensionError": {
      "description": "Invalid dimensions for OCR",
      "message": "The parameter is incorrect.Image size is not supported. Each image dimension must be between 40 and 2600 pixels."
   },
   "ocrProgressStatus": {
      "description": "OCR conversion in progress",
      "message": "OCR conversion in progress ..."
   },
   "ocrProgressStatusStillWorking": {
      "description": "Still working hard",
      "message": "Still working hard ..."
   },
   "ocrSuccessStatus": {
      "description": "OCR was successful",
      "message": "OCR successful"
   },
   "overlayTab": {
      "description": "Title of the overlay page",
      "message": "Copyfish Result Overlay"
   },
   "permWarning": {
      "description": "Error message when user denies permission",
      "message": "Please grant permission to use this feature"
   },
   "screenCapture": {
      "description": "Title of the screen capture page",
      "message": "Screen Capture"
   },
   "screenCaptureNotify": {
      "description": "Click on the extension icon to begin selection",
      "message": "Click on the extension icon to begin selection."
   },
   "screenCaptureWaitMessage": {
      "description": "Waiting for screen capture . . .",
      "message": "Waiting for screen selection . . ."
   },
   "translationProgressStatus": {
      "description": "Translation in progress",
      "message": "Translation in progress ..."
   },
   "translationSuccessStatus": {
      "description": "Translation was successful",
      "message": "Translation successful"
   },
   "userMediaUnsupported": {
      "description": "getUserMedia is unsupported",
      "message": "Your browser does not support desktop capture"
   }
}



================================================
FILE: config/config.json
================================================
{
    "defaults": {
        "visualCopyOCRLang": "eng",
        "visualCopyTranslateLang": "en",
        "visualCopyAutoTranslate": false,
        "visualCopyOCRFontSize": "16px",
        "visualCopySupportDicts": false,
        "copyAfterProcess": true,
        "visualCopyQuickSelectLangs": [],
        "visualCopyTextOverlay": true,
        "openGrabbingScreenHotkey": 49,
        "closePanelHotkey": 50,
        "copyTextHotkey": 51,
        "ocrEngine": "OcrSpaceSecond",
        "copyType": "Text",
        "transitionEngine": false,
        "status": "Free Plan",
        "useTableOcr": false,
        "useDefaultDesktopOcr":false
    },
    "yandex_api_key": "trnsl.1.1.k1.86128cd59209eaf8.513c1afcbd4eaa561318196f5d48450fcfa42215",
    "yandex_api_url": "https://translate.yandex.net/api/v1.5/tr.json/translate",
    "ocr_api_list": [
        {
            "id": "1",
            "ocr_api_key": "test",
            "ocr_api_url": "https://apipro1.ocr.space/parse/image"
        },
        {
            "id": "2",
            "ocr_api_key": "test",
            "ocr_api_url": "https://apipro2.ocr.space/parse/image"
        },
	    {
            "id": "3",
            "ocr_api_key": "test",
            "ocr_api_url": "https://apipro3.ocr.space/parse/image"
        }
    ],
    "ocr_server_reset_time": 86400,
    "ocr_timeout": 15000,
    "yandex_timeout": 30000,
    "google_translation_api_url":"https://translation.googleapis.com/language/translate/v2",
    "deepapi_translation_api_url":"https://api.deepl.com/v2/translate",
    "yandex_err_msgs": {
        "401": "ERR_KEY_INVALID",
        "402": "ERR_KEY_BLOCKED",
        "403": "ERR_DAILY_REQ_LIMIT_EXCEEDED",
        "404": "ERR_DAILY_CHAR_LIMIT_EXCEEDED",
        "413": "ERR_TEXT_TOO_LONG",
        "422": "ERR_UNPROCESSABLE_TEXT",
        "501": "ERR_LANG_NOT_SUPPORTED"
    },
    "ocr_google_languages": [
        {
            "lang": "auto",
            "name": "Auto-Detect",
            "short": "AUTO"
        },
        {
            "lang": "af",
            "name": "Afrikaans",
            "short": "AF"
        },
        {
            "lang": "ar",
            "name": "Arabic",
            "short": "AR"
        },
        {
            "lang": "as",
            "name": "Assamese",
            "short": "AS"
        },
        {
            "lang": "az",
            "name": "Azerbaijani",
            "short": "AZ"
        },
        {
            "lang": "be",
            "name": "Belarusian",
            "short": "BE"
        },
        {
            "lang": "bn",
            "name": "Bengali",
            "short": "BN"
        },
        {
            "lang": "bg",
            "name": "Bulgarian",
            "short": "BG"
        },
        {
            "lang": "ca",
            "name": "Catalan",
            "short": "CA"
        },
        {
            "lang": "zh*",
            "name": "Chinese",
            "short": "CH"
        },
        {
            "lang": "hr",
            "name": "Croatian",
            "short": "CR"
        },
        {
            "lang": "cs",
            "name": "Czech",
            "short": "CZ"
        },
        {
            "lang": "da",
            "name": "Danish",
            "short": "DA"
        },
        {
            "lang": "nl",
            "name": "Dutch",
            "short": "DT"
        },
        {
            "lang": "en",
            "name": "English",
            "short": "EN"
        },
        {
            "lang": "et",
            "name": "Estonian",
            "short": "ET"
        },
        {
            "lang": "fil",
            "name": "Filipino",
            "short": "FIL"
        },
        {
            "lang": "fi",
            "name": "Finnish",
            "short": "FI"
        },
        {
            "lang": "fr",
            "name": "French",
            "short": "FR"
        },
        {
            "lang": "de",
            "name": "German",
            "short": "GE"
        },
        {
            "lang": "el",
            "name": "Greek",
            "short": "GR"
        },
        {
            "lang": "he",
            "name": "Hebrew",
            "short": "HE"
        },
        {
            "lang": "hi",
            "name": "Hindi",
            "short": "HI"
        },
        {
            "lang": "hu",
            "name": "Hungarian",
            "short": "HU"
        },
        {
            "lang": "is",
            "name": "Icelandic",
            "short": "IS"
        },
        {
            "lang": "id",
            "name": "Indonesian",
            "short": "ID"
        },
        {
            "lang": "it",
            "name": "Italian",
            "short": "IT"
        },
        {
            "lang": "ja",
            "name": "Japanese",
            "short": "JA"
        },
        {
            "lang": "kk",
            "name": "Kazakh",
            "short": "KZ"
        },
        {
            "lang": "ko",
            "name": "Korean",
            "short": "KO"
        },
        {
            "lang": "ky",
            "name": "Kyrgyz",
            "short": "KY"
        },
        {
            "lang": "lv",
            "name": "Latvian",
            "short": "LV"
        },
        {
            "lang": "lt",
            "name": "Lithuanian",
            "short": "LT"
        },
        {
            "lang": "mk",
            "name": "Macedonian",
            "short": "MK"
        },
        {
            "lang": "mr",
            "name": "Marathi",
            "short": "MR"
        },
        {
            "lang": "mn",
            "name": "Mongolian",
            "short": "MN"
        },
        {
            "lang": "ne",
            "name": "Nepali",
            "short": "NE"
        },
        {
            "lang": "no",
            "name": "Norwegian",
            "short": "NO"
        },
        {
            "lang": "ps",
            "name": "Pashtu",
            "short": "PS"
        },
        {
            "lang": "fa",
            "name": "Persian",
            "short": "PE"
        },
        {
            "lang": "pl",
            "name": "Polish",
            "short": "PL"
        },
        {
            "lang": "pt",
            "name": "Portuguese",
            "short": "PT"
        },
        {
            "lang": "ro",
            "name": "Romanian",
            "short": "RO"
        },
        {
            "lang": "ro",
            "name": "Romanian",
            "short": "RO"
        },
        {
            "lang": "ru",
            "name": "Russian",
            "short": "RU"
        },
        {
            "lang": "sa",
            "name": "Sanskrit",
            "short": "SA"
        },
        {
            "lang": "sr",
            "name": "Serbian",
            "short": "SR"
        },
        {
            "lang": "sk",
            "name": "Slovak",
            "short": "SK"
        },
        {
            "lang": "sl",
            "name": "Slovenian",
            "short": "SL"
        },
        {
            "lang": "es",
            "name": "Spanish",
            "short": "SP"
        },
        {
            "lang": "sv",
            "name": "Swedish",
            "short": "SW"
        },
        {
            "lang": "ta",
            "name": "Tamil",
            "short": "TA"
        },
        {
            "lang": "th",
            "name": "Thai",
            "short": "TH"
        },
        {
            "lang": "tr",
            "name": "Turkish",
            "short": "TR"
        },
        {
            "lang": "uk",
            "name": "Ukrainian",
            "short": "UK"
        },
        {
            "lang": "ur",
            "name": "Urdu",
            "short": "UR"
        },
        {
            "lang": "uz",
            "name": "Uzbek",
            "short": "UZ"
        },
        {
            "lang": "vi",
            "name": "Vietnamese",
            "short": "VI"
        }
    ],
    "google_languages": [
        {
            "en": "English"
        },
        {
            "af": "Afrikaans"
        },
        {
            "sq": "Albanian"
        },
        {
            "am": "Amharic"
        },
        {
            "ar": "Arabian"
        },
        {
            "hy": "Armenian"
        },
        {
            "az": "Azerbaijani"
        },
        {
            "eu": "Basque"
        },
        {
            "be": "Belarusian"
        },
        {
            "bn": "Bosnian"
        },
        {
            "bs": "Bengali"
        },
        {
            "bg": "Bulgarian"
        },
        {
            "ca": "Catalan"
        },
        {
            "hr": "Croatian"
        },
        {
            "cs": "Czech"
        },
        {
            "zh-TW": "Chinese"
        },
        {
            "zh-CN": "Chinese Simplified"
        },
        {
            "da": "Danish"
        },
        {
            "ceb": "Cebuano"
        },
        {
            "nl": "Dutch"
        },
        {
            "et": "Estonian"
        },
        {
            "fi": "Finnish"
        },
        {
            "fr": "French"
        },
        {
            "gl": "Galician"
        },
        {
            "ka": "Georgian"
        },
        {
            "de": "German"
        },
        {
            "el": "Greek"
        },
        {
            "ht": "Haitian (Creole)"
        },
        {
            "iw": "Hebrew"
        },
        {
            "hu": "Hungarian"
        },
        {
            "is": "Icelandic"
        },
        {
            "id": "Indonesian"
        },
        {
            "ga": "Irish"
        },
        {
            "it": "Italian"
        },
        {
            "ja": "Japanese"
        },
        {
            "kk": "Kazakh"
        },
        {
            "ko": "Korean"
        },
        {
            "ky": "Kyrgyz"
        },
        {
            "la": "Latin"
        },
        {
            "lv": "Latvian"
        },
        {
            "lt": "Lithuanian"
        },
        {
            "mk": "Macedonian"
        },
        {
            "mg": "Malagasy"
        },
        {
            "ms": "Malay"
        },
        {
            "mt": "Maltese"
        },
        {
            "mn": "Mongolian"
        },
        {
            "no": "Norwegian"
        },
        {
            "fa": "Persian"
        },
        {
            "pl": "Polish"
        },
        {
            "pt": "Portuguese"
        },
        {
            "ro": "Romanian"
        },
        {
            "ru": "Russian"
        },
        {
            "es": "Spanish"
        },
        {
            "sr": "Serbian"
        },
        {
            "sk": "Slovak"
        },
        {
            "sl": "Slovenian"
        },
        {
            "sw": "Swahili"
        },
        {
            "sv": "Swedish"
        },
        {
            "tl": "Tagalog"
        },
        {
            "tg": "Tajik"
        },
        {
            "tt": "Tatar"
        },
        {
            "ta": "Tamil"
        },
        {
            "te": "Telugu"
        },
        {
            "ur": "Urdu"
        },
        {
            "th": "Thai"
        },
        {
            "tr": "Turkish"
        },
        {
            "uz": "Uzbek"
        },
        {
            "uk": "Ukrainian"
        },
        {
            "vi": "Vietnamese"
        },
        {
            "cy": "Welsh"
        },
        {
            "xh": "Xhosa"
        },
        {
            "yi": "Yiddish"
        },
        {
            "yo": "Yoruba"
        },
        {
            "zu": "Zulu"
        }
    ],
    "deepapi_languages": [
        {
            "en": "English"
        },
        {
            "de": "German"
        },
        {
            "fr": "French"
        },
        {
            "es": "Spanish"
        },
        {
            "pt": "Portuguese"
        },
        {
            "it": "Italian"
        },
        {
            "nl": "Dutch"
        },
        {
            "pl": "Polish"
        },
        {
            "ru": "Russian"
        },
        {
            "ja": "Japanese"
        },
        {
            "zh": "Chinese"
        }
    ],
    "ocr_languages": [
        {
            "lang": "eng",
            "name": "English",
            "short": "EN"
        },
        {
            "lang": "ara",
            "name": "Arabic",
            "short": "AR"
        },
        {
            "lang": "chs",
            "name": "ChineseSimplified",
            "short": "简"
        },
        {
            "lang": "cht",
            "name": "ChineseTraditional",
            "short": "繁"
        },
        {
            "lang": "ce",
            "name": "Czech",
            "short": "CS"
        },
        {
            "lang": "dan",
            "name": "Danish",
            "short": "DA"
        },
        {
            "lang": "dut",
            "name": "Dutch",
            "short": "NL"
        },
        {
            "lang": "fin",
            "name": "Finnish",
            "short": "FI"
        },
        {
            "lang": "fre",
            "name": "French",
            "short": "FR"
        },
        {
            "lang": "ger",
            "name": "German",
            "short": "DE"
        },
        {
            "lang": "gre",
            "name": "Greek",
            "short": "EL"
        },
        {
            "lang": "hun",
            "name": "Hungarian",
            "short": "HU"
        },
        {
            "lang": "ita",
            "name": "Italian",
            "short": "IT"
        },
        {
            "lang": "jpn",
            "name": "Japanese",
            "short": "JP"
        },
        {
            "lang": "kor",
            "name": "Korean",
            "short": "KO"
        },
        {
            "lang": "nor",
            "name": "Norwegian",
            "short": "NN"
        },
        {
            "lang": "pol",
            "name": "Polish",
            "short": "PL"
        },
        {
            "lang": "por",
            "name": "Portuguese",
            "short": "PT"
        },
        {
            "lang": "rus",
            "name": "Russian",
            "short": "RU"
        },
        {
            "lang": "spa",
            "name": "Spanish",
            "short": "ES"
        },
        {
            "lang": "swe",
            "name": "Swedish",
            "short": "SV"
        },
        {
            "lang": "tur",
            "name": "Turkish",
            "short": "TR"
        }
    ],
    "yandex_languages": [
        {
            "en": "English"
        },
        {
            "af": "Afrikaans"
        },
        {
            "sq": "Albanian"
        },
        {
            "ar": "Arabian"
        },
        {
            "hy": "Armenian"
        },
        {
            "az": "Azeri"
        },
        {
            "eu": "Basque"
        },
        {
            "be": "Belarusian"
        },
        {
            "bs": "Bosnian"
        },
        {
            "bg": "Bulgarian"
        },
        {
            "ca": "Catalan"
        },
        {
            "hr": "Croatian"
        },
        {
            "cs": "Czech"
        },
        {
            "zh-TW": "Chinese"
        },
        {
            "zh-CN": "Chinese Simplified"
        },
        {
            "da": "Danish"
        },
        {
            "co": "Corsican"
        },
        {
            "nl": "Dutch"
        },
        {
            "et": "Estonian"
        },
        {
            "fi": "Finnish"
        },
        {
            "fr": "French"
        },
        {
            "gl": "Galician"
        },
        {
            "ka": "Georgian"
        },
        {
            "de": "German"
        },
        {
            "el": "Greek"
        },
        {
            "ht": "Haitian (Creole)"
        },
        {
            "ha": "Hausa"
        },
        {
            "he": "Hebrew"
        },
        {
            "haw": "Hawaiian"
        },
        {
            "hmn": "Hmong"
        },
        {
            "hu": "Hungarian"
        },
        {
            "gu": "Gujarati"
        },
        {
            "is": "Icelandic"
        },
        {
            "id": "Indonesian"
        },
        {
            "ig": "Igbo"
        },
        {
            "ga": "Irish"
        },
        {
            "it": "Italian"
        },
        {
            "ja": "Japanese"
        },
        {
            "kn": "Kannada"
        },
        {
            "kk": "Kazakh"
        },
        {
            "ko": "Korean"
        },
        {
            "km": "Khmer"
        },
        {
            "ky": "Kyrgyz"
        },
        {
            "ku": "Kurdish"
        },
        {
            "la": "Latin"
        },
        {
            "lo": "Lao"
        },
        {
            "lb": "Luxembourgish"
        },
        {
            "lv": "Latvian"
        },
        {
            "lt": "Lithuanian"
        },
        {
            "mk": "Macedonian"
        },
        {
            "mg": "Malagasy"
        },
        {
            "ms": "Malay"
        },
        {
            "mt": "Maltese"
        },
        {
            "mi": "Marathi"
        },
        {
            "mn": "Mongolian"
        },
        {
            "ne": "Nepali"
        },
        {
            "ny": "Nyanja (Chichewa)"
        },
        {
            "ps": "Pashto"
        },
        {
            "no": "Norwegian"
        },
        {
            "fa": "Persian"
        },
        {
            "pl": "Polish"
        },
        {
            "pt": "Portuguese"
        },
        {
            "pa": "Punjabi"
        },
        {
            "ro": "Romanian"
        },
        {
            "ru": "Russian"
        },
        {
            "es": "Spanish"
        },
        {
            "sm": "Samoan"
        },
        {
            "gd": "Scots Gaelic"
        },
        {
            "sr": "Serbian"
        },
        {
            "st": "Sesotho"
        },
        {
            "sn": "Shona"
        },
        {
            "sd": "Sindhi"
        },
        {
            "si": "Sinhala (Sinhalese)"
        },
        {
            "sk": "Slovak"
        },
        {
            "sl": "Slovenian"
        },
        {
            "so": "Somali"
        },
        {
            "su": "Sundanese"
        },
        {
            "sw": "Swahili"
        },
        {
            "sv": "Swedish"
        },
        {
            "tl": "Tagalog"
        },
        {
            "tg": "Tajik"
        },
        {
            "tt": "Tatar"
        },
        {
            "th": "Thai"
        },
        {
            "tr": "Turkish"
        },
        {
            "uz": "Uzbek"
        },
        {
            "uk": "Ukrainian"
        },
        {
            "vi": "Vietnamese"
        },
        {
            "cy": "Welsh"
        }
    ]
}


================================================
FILE: scripts/chromereload.js
================================================
// 'use strict';
//
// // Reload client for Chrome Apps & Extensions.
// // The reload client has a compatibility with livereload.
// // WARNING: only supports reload command.
//
// var LIVERELOAD_HOST = 'userver:';
// var LIVERELOAD_PORT = 35729;
// var connection = new WebSocket('ws://' + LIVERELOAD_HOST + LIVERELOAD_PORT + '/livereload');
//
// connection.onerror = function (error) {
// 	console.log('reload connection got error:', error);
// };
//
// connection.onmessage = function (e) {
// 	if (e.data) {
// 		var data = JSON.parse(e.data);
// 		if (data && data.command === 'reload') {
// 			chrome.runtime.reload();
// 		}
// 	}
// };



================================================
FILE: scripts/crossbrowser.js
================================================
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

/**
 * Detects what browser the extension is running on
 * (Currently, all Chromium browsers are listed under Chrome)
 * @returns Browser enum
 */
function detectBrowser() {
    if (typeof browser != "object") browser = chrome;

    if (browser.runtime.getURL('').startsWith('moz-extension://')) {
        return browsers.FIREFOX;
    } else if (browser.runtime.getURL('').startsWith('edge://extension')) {
        return browsers.EDGE;
    } else {
        return browsers.CHROME;
    }
}

/**
 * Get the name of the browser the user is running
 * @returns Browser Name
 */
function getBrowserName() {
    const names = [
        'Firefox',
        'Chrome',
        'Edge',
        'Opera'
    ];

    return names[runningOn];
}

const browsers = {
    FIREFOX: 0,
    CHROME: 1,
    EDGE: 2,
    OPERA: 3
};
const runningOn = detectBrowser();



================================================
FILE: scripts/genlib.js
================================================
const NMHOST = 'com.github.teamdocs.kcmd';

function createNMPromise(command) {
	return new Promise(resolve => browser.runtime.sendNativeMessage(NMHOST, { command: command }, response => {
		resolve(response);
	}));
}



================================================
FILE: scripts/init-cs.js
================================================
/* This content script is being loaded by default by manifest.json in firefox only*/

(function () {
    'use strict';
    let onDemandFunc = {
        init: function () {
            browser.runtime.onMessage.addListener((request, sender, sendResponse) => {
                if (sender.tab) {
                    return true;
                }
                if (request.evt === 'captureClipboard') {
                    this.captureClipboard(sendResponse);
                } else if (request.evt === 'copyToClipboard') {
                    this.copyToClipboard(request, sendResponse);
                }
            });
        },
        checkValidImgBase64: function (s) {
            let regex = /^\s*data:([a-z]+\/[a-z]+(;[a-z\-]+\=[a-z\-]+)?)?(;base64)?,[a-z0-9\!\$\&\'\,\(\)\*\+\,\;\=\-\.\_\~\:\@\/\?\%\s]*\s*$/i;
            return s.match(regex);
        },
        toDataURL: function (url) {
            return new Promise((resolve, reject) => {
                try {
                    var xhr = new XMLHttpRequest();
                    xhr.onload = function () {
                        var reader = new FileReader();
                        reader.onloadend = function () {
                            resolve(reader.result);
                        }
                        reader.readAsDataURL(xhr.response);
                    };
                    xhr.open('GET', url);
                    xhr.responseType = 'blob';
                    xhr.send();
                }
                catch (err) {
                    return reject(err);
                }
            });
        },
        captureClipboard: function (sendResponse) {
            navigator.clipboard.readText().then(function (text) {
            });
            let imgSrcRegex = /<img[^>]+src="([^">]+)"/g;
            let copyDiv = document.createElement('div');
            copyDiv.style.width = '1px';
            copyDiv.style.height = '1px';
            copyDiv.style.opacity = 0;
            copyDiv.contentEditable = true;
            document.body.appendChild(copyDiv);
            copyDiv.focus();
            document.execCommand("paste");
            let imageContent = copyDiv.innerHTML;
            copyDiv.remove();
            let src = imgSrcRegex.exec(imageContent);
            if (!src || !src[ 1 ]) {
                browser.runtime.sendMessage({
                    evt: 'show-warning-message',
                    data: { message: 'No image in clipboard' },
                });
                return;
            }
            if(src && src[ 1 ] && !this.checkValidImgBase64(src[ 1 ])){
                this.toDataURL(src[ 1 ]).then((res)=>{
                    browser.runtime.sendMessage({
                        evt: 'imageOcrInTab',
                        ocrText: '',
                        overlayInfo: '',
                        data: res,
                        translatedTextIfAny: '',
                        currentZoomLevel: 0,
                    });
                },(err)=>{
                    browser.runtime.sendMessage({
                        evt: 'show-warning-message',
                        data: { message: 'No image in clipboard' },
                    });
                    return;    
                });
                return;
            }
            else if (!this.checkValidImgBase64(src[ 1 ])) {
                browser.runtime.sendMessage({
                    evt: 'show-warning-message',
                    data: { message: 'No image in clipboard' },
                });
                return;
            }
            browser.runtime.sendMessage({
                evt: 'imageOcrInTab',
                ocrText: '',
                overlayInfo: '',
                data: src[ 1 ],
                translatedTextIfAny: '',
                currentZoomLevel: 0,
            });
        }, copyToClipboard: function (request, sendResponse) {
            let copyDivElm = document.createElement('div');
            copyDivElm.contentEditable = true;
            copyDivElm.style.opacity = 0;
            copyDivElm.style = "white-space:pre-wrap;"
            document.body.appendChild(copyDivElm);
            copyDivElm.textContent = request && request.data || '';
            copyDivElm.unselectable = 'off';
            copyDivElm.focus();
            document.execCommand('SelectAll');
            document.execCommand('Copy', false, null);
            document.body.removeChild(copyDivElm);
            request.onComplete && request.onComplete();
        }
    }
    onDemandFunc.init();
}());



================================================
FILE: scripts/message-dialog.js
================================================
let messageDialog = (function () {
    let funBlock = {
        init: function () {
            let self = this;
            $(function () {
                $('#downloadHelper').on('click', function () {
                    self.downloadHelperApp();
                });
                $('#downloadReadMore,#readMoreLink').on('click', function () {
                    self.readMore();
                });
                $('body').on('click', '#takeDesktopScreenshot', function () {
                    self.desktopScreenshot();
                });
                $('body').on('click', '#tryWebScreenshot', function () {
                    self.webScreenshot();
                });
                $('body').on('click', '#cancelPopup', function () {
                    self.closePopup();
                });
                self.notLoadedDialog();
            });
        },
        desktopScreenshot: function () {
           
        },
        webScreenshot: function () {
           
        },
        notLoadedDialog: function () {
            let param = this.parseUrlParam('forLoadingPopup');
            if (param == 'on') {
                let message = `
                If this message does not disappear, try reloading the page. Or use the desktop screenshot feature.
                <div class="button-row btn-center">
                <span>
                    <button id='cancelPopup'
                        class="cp-show-dialog-button ocrext-btn mdl-button mdl-js-button mdl-button--raised mdl-js-ripple-effect mdl-button--accent"
                        title="">
                        Close
                    </button>
                </span>
                </div>
                `;
                $('#cp-dialog-description').html(message);
            }
        },
        parseUrlParam: function (param) {
            try {
                let url = new URL(window.location.href);
                return url.searchParams.get(param);
            }
            catch (err) {
                return '';
            }
        },
        downloadHelperApp: function () {
            this.closePopup();
            window.open('https://ui.vision/rpa/x/download', '_blank');
        },
        readMore: function () {
            this.closePopup();
            window.open('https://ocr.space/rd/copyfish?help=desktop', '_blank');
        },
        closePopup: function () {
            window.close();
        }
    }
    funBlock.init();
    return funBlock;
})();



================================================
FILE: scripts/ocrlocal.js
================================================
'use strict';

let dataUrls = [];

tabbis({
	tabActiveFallback: 0
});

document.addEventListener("tabbis", e => {
	let data = e.detail.tab;

	if ($(data).attr('id') === 'tab-0-0') {
		$('#tabpanel-0-0').removeClass('canvas-hidden')
	} else {
		$('#tabpanel-0-0').addClass('canvas-hidden')
	}
}, false);


let LocalOcr = {

	sum: (...list) => {
		return list.reduce((x, y) => x + y, 0);
	},

	blobToDataURL: (blob, withBase64Prefix = false) => {
		return new Promise((resolve, reject) => {
			let reader = new FileReader()
			reader.onerror = reject
			reader.onload = (e) => {

				const str = reader.result

				const b64 = 'base64,'
				const i = str.indexOf(b64)
				const ret = str.substr(i + b64.length)

				$('.copyfish-image-view').attr('src', 'data:image/png;base64, ' + ret)
				resolve(ret)
			}
			reader.readAsDataURL(blob)
		})
	},

	dataURItoArrayBuffer: (dataURI) => {
		// convert base64 to raw binary data held in a string
		// doesn't handle URLEncoded DataURIs - see SO answer #6850276 for code that does this
		var byteString = atob(
			/^data:/.test(dataURI) ? dataURI.split(',')[ 1 ] : dataURI
			);

		// write the bytes of the string to an ArrayBuffer
		var ab = new ArrayBuffer(byteString.length);

		// create a view into the buffer
		var ia = new Uint8Array(ab);

		// set the bytes of the buffer to the correct values
		for (var i = 0; i < byteString.length; i++) {
			ia[ i ] = byteString.charCodeAt(i);
		}

		return ab
	},

	concatUint8Array: (...arrays) => {
		const totalLength = LocalOcr.sum(...arrays.map(arr => arr.length));
		const result = new Uint8Array(totalLength);
		for (let i = 0, offset = 0, len = arrays.length; i < len; i += 1) {
			result.set(arrays[ i ], offset);
			offset += arrays[ i ].length;
		}
		return result;
	},

	readFileAsArrayBuffer: (range) => {

		return new Promise((resolve, reject) => {
			const result = range.rangeEnd > range.rangeStart ? dataUrls.concat([ range.buffer ]) : dataUrls;

			console.log(dataUrls, 12312312);
			const arr = LocalOcr.concatUint8Array(...result.map(result => new Uint8Array(LocalOcr.dataURItoArrayBuffer(result))));
			console.log(arr.buffer, 12312312);
			resolve(arr.buffer);
		});
	},

	readFileAsBlob: (range) => {
		return new Promise((resolve, reject) => {
			resolve(LocalOcr.readFileAsArrayBuffer(range)
				.then(buffer => new Blob([ buffer ])));
		});

	},

	readFileAsDataURL: (range, withBase64Prefix = true) => {

		return LocalOcr.readFileAsBlob(range)
		.then(blob => LocalOcr.blobToDataURL(blob, withBase64Prefix));
	},

	init: function () {
		this.title = browser.i18n.getMessage('appName') + ' - ' + browser.i18n.getMessage('screenCapture');
		$('title,.title').text(this.title);
		$('.placeholder').text(browser.i18n.getMessage('screenCaptureWaitMessage'));
		$('.settings-icon').click((e) => {
			e.stopPropagation();
			browser.runtime.sendMessage({
				evt: 'open-settings'
			});
		})

		function resizedataURL(datas) {
			var imageLoadDfd = $.Deferred();
			// We create an image to receive the Data URI
			var img = document.createElement('img');
			var devicePxRatio = devicePixelRatio;
			var scaleValue = 1 / devicePxRatio;
			// When the event "onload" is triggered we can resize the image.
			img.onload = function () {
				// We create a canvas and get its context.
				var canvas = document.createElement('canvas');
				var ctx = canvas.getContext('2d');
				var wantedWidth = img.width * scaleValue;
				var wantedHeight = img.height * scaleValue;
				// We set the dimensions at the wanted size.
				canvas.width = wantedWidth;
				canvas.height = wantedHeight;
				// We resize the image with the canvas method drawImage();
				ctx.drawImage(this, 0, 0, wantedWidth, wantedHeight);
				var dataURI = canvas.toDataURL();
				imageLoadDfd.resolve(dataURI);
				//return dataURI;
				/////////////////////////////////////////
				// Use and treat your Data URI here !! //
				/////////////////////////////////////////
			};
			img.src = datas;
			// We put the Data URI in the image's src attribute

			return imageLoadDfd;
		}
		function copyToClipboard(request, sendResponse) {
			let copyDivElm = document.createElement('div');
			copyDivElm.contentEditable = true;
			copyDivElm.style.opacity = 0;
			copyDivElm.style = "white-space:pre-wrap;"
			document.body.appendChild(copyDivElm);
			copyDivElm.textContent = request && request.data || '';
			copyDivElm.unselectable = 'off';
			copyDivElm.focus();
			document.execCommand('SelectAll');
			document.execCommand('Copy', false, null);
			document.body.removeChild(copyDivElm);
			request.onComplete && request.onComplete();
		}

		browser.runtime.onMessage.addListener(function (request, sender, sendResponse) {
			//console.log(request.evt, 8789)
				console.log(request)
			if (request.evt === 'desktopcaptureLocal') {
						if (request.result.buffer) {
							resizedataURL(`data:application/octet-stream;base64,${request.result.buffer}`).done(function (resultDataUri) {
								request.result.buffer = resultDataUri;
								$('#copyfish-tab-image').attr('src', `${request.result.buffer}`)
								browser.runtime.sendMessage({ evt: "translateDesktopCapturedImage", data: `${request.result.buffer}`,imagepath: `${request.imagepath}`})
								const setImage = LocalOcr.readFileAsDataURL(request.result);
								$('.placeholder')
								.text(browser.i18n.getMessage('screenCaptureNotify'))
								.addClass('notify');
							});
						} else {
							$('#copyfish-tab-image').attr('src', request.result);
							browser.runtime.sendMessage({ 	evt: "translateDesktopCapturedImage",
								imagepath: request.imagepath,
								data: request.result,
								ocrText: request.ocrText || '',
								overlayInfo : request.overlayInfo || '',
								forExternalTab:request.forExternalTab || 0,
								translatedTextIfAny	: request.translatedTextIfAny || '',
								currentZoomLevel	: request.currentZoomLevel || 0,
							})
						}	
					}
				
			return true;
		});
	}
};
$(function(){
	$(LocalOcr.init);
});





================================================
FILE: scripts/overlay.js
================================================
window.browser = (function () {
	return window.msBrowser ||
		window.browser ||
		window.chrome;
})();

//let isFirefox = typeof InstallTrigger !== 'undefined';
(function () {
	'use strict';
	const htmlDialog = function () {
		const allMethod = {
			init: function () {
				let self = this;
				$('body').off('click', '[popup-close]');
				$('body').off('click', '.cp-dialog-close-button,.cp-dialog-popup');
				$('body').on('click', '[popup-close]', function () {
					var popup_name = $(this).attr('popup-close');
					$('[popup-name="' + popup_name + '"]').fadeOut(300);
				});
				// Close Popup When Click Outside
				$('body').on('click', '.cp-dialog-close-button', function () {
					var popup_name = $(this).find('[popup-close]').attr('popup-close');
					$('[popup-name="' + popup_name + '"]').fadeOut(300);
					$(this).children().click(function () {
						return false;
					});
				});
				$(document).on('keyup', function (e) {
					if (e.keyCode === 27) {
						self.closeDialog();
					}
				});
				$('body').attr('data-ocrext-dialog',1);
			},
			closeDialog: function(){
				$('#cfish-popup-message-dialog').fadeOut(300);
				let isfounfLocalocr = $("#OcrLocal").attr('LocalOcrFound');
				if ($("#OcrLocal").prop('checked') && isfounfLocalocr == 'NO') {
					$('#OcrSpace').click()
				}
			},
			hardClose:function(){
				$('#cfish-popup-message-dialog').hide();
			},
			showDialog: function (header,message, buttons) {
				let buttonHtml = '';
				//this.hardClose();
				buttons && buttons.forEach((single,i) => {
					let { label = '', cb = () => { } } = single;
					let buttonId = 'cfish-' + i + (Date.now());
					let btn = '<span><button id="' + buttonId + '" class="cp-show-dialog-button ocrext-btn mdl-button mdl-js-button mdl-button--raised mdl-js-ripple-effect mdl-button--accent" title="">' + label + '</button></span>';
					buttonHtml += btn;
					if ($('#' + buttonId).length) {
						$('#' + buttonId).remove();
					}
					$('body').off('click', '#' + buttonId);
					$('body').on('click', '#' + buttonId, cb);
				});
				$('#cp-dialog-title').html('');
				$('#cp-dialog-description').html('');
				$('#cp-dialog-image').attr('src', browser.runtime.getURL("images/copyfish-32.png"));
				$('#cp-dialog-title').html(header);
				$('#cp-dialog-description').html(message);
				buttonHtml && $('#cp-dialog-description').append('<div class="button-row ' + (buttons.length == 1 ? 'btn-center' : '') + '">' + buttonHtml + '</div>');
				$('[popup-name="popup-1"]').fadeIn(300);
			},
		}

		return allMethod;
	};

	var TextOverlay = function () {
		var _overlay;
		var $container;
		var htmlString;
		var wordString;
		var $overlay;
		var _overlayInstance;
		var _init;



		var _isOverlayAvailable = function () {
			return !!_overlay && _overlay.HasOverlay;
		};
		$container = $('.ocrext-textoverlay-container');

		htmlString = [
			'<div class="ocrext-element ocrext-text-overlay">',
			'<div class="ocrext-element ocrext-text-overlay-word-wrapper">',
			'<img class="ocrext-element ocrext-text-overlay-img text-overlay-img" />',
			'</div>',
			'</div>'
		].join('');
		wordString = '<span class="ocrext-element ocrext-text-overlay-word"></span>';

		_init = function (self) {
			var run;
			// $('title,.title').text(browser.i18n.getMessage('appName') + ' - ' + browser.i18n.getMessage('overlayTab'));
			// `self` is passed; pythonic!
			if($container && $container.length){
				// reset if already available
				$container.find('.ocrext-text-overlay').remove();
			}
			$(htmlString).appendTo($container);
			$overlay = $('.ocrext-textoverlay-container')

			$container.on('click', '.ocrext-close-link', function () {
				_overlayInstance.hide();
			});
		};

		_overlayInstance = {

			setOverlayInformation: function (overlayInfo, canvasWidth, canHeight, imgDataURI, zoom) {
				// if setOverlayInformation is called when _overlay is already set, do nothing!
				if (!_overlay) {
					_overlay = overlayInfo;
					this.render(canvasWidth, canHeight, imgDataURI, zoom);
				}
				return this;
			},
			getOverlayInformation: function () {
				return _overlay;
			},
			render: function (canvasWidth, canvasHeight, imgDataURI, zoom) {
				zoom = zoom || 1;
				if (_isOverlayAvailable()) {
					var lines = _overlay.Lines;
					var $wordWrapper = $overlay.find('.ocrext-text-overlay-word-wrapper');
					var $word;
					if (imgDataURI) {
						$container.find('.text-overlay-img').attr('src', imgDataURI);
					}

					this.setDimensions(canvasWidth, canvasHeight);
					$.each(lines, function (i, line) {
						var maxLineHeight = line.MaxHeight * zoom;
						var minLineTopDist = line.MinTop * zoom;
						$.each(line.Words, function (j, word) {
							$word = $(wordString);
							$word
								.text(word.WordText)
								.css({
									left: word.Left * zoom,
									top: minLineTopDist,
									height: maxLineHeight,
									width: word.Width * zoom,
									fontSize: maxLineHeight * 0.7
								})
								.appendTo($wordWrapper);
							$word = null;
						});

					});
				}
				return this;
			},

			setDimensions: function (width, height) {

				$.each([$overlay, $overlay.find('.ocrext-text-overlay-word-wrapper')], function () {
					this.width(width).height(height);
				});

				return this;
			},

			reset: function () {
				_overlay = null;
				$overlay.find('.ocrext-text-overlay-word-wrapper span').remove();
				return this;
			},

			show: function () {
				if (_isOverlayAvailable()) {
					// this.position();
					$container.addClass('visible');
					$overlay.addClass('visible');
					$container.find('.ocrext-text-overlay').addClass('visible');

				} else {
					// logError('Overlay is unavailable.');
					// window.alert('Sorry. Text overlay is currently unavailable.');
				}
				return this;
			},

			hide: function () {
				$container.removeClass('visible');
				$overlay.removeClass('visible');
				$container.find('.ocrext-text-overlay').remove('visible');
				return this;
			},

			position: function () {
				var bodyWidth, bodyHeight;
				var $body = $('body');
				bodyWidth = $body.width();
				bodyHeight = $(window).height();
				$overlay.css({
					left: bodyWidth / 2 - $overlay.width() / 2,
					top: 150
				});
				return this;
			},

			setTitle: function () {
				$('title,.ocrext-textoverlay-title').text(browser.i18n.getMessage('overlayTab'));
				return this;
			},

			listenToBackgroundEvents: function () {
				var self = this;
				browser.runtime.onMessage.addListener(function (request, sender, sendResponse) {

					console.log(request.evt, 8789)

					if (sender.tab) {
						return true;
					}
					
					if (request.evt === 'init-overlay-tab') {
						self.setOverlayInformation(request.overlayInfo, request.canWidth, request.canHeight, request.imgDataURI, request.zoom);
						// self.position();
						self.show();
						sendResponse({
							farewell: 'init-overlay-tab:OK'
						});
						return true;
					}
				});
			}

		};
		_init(_overlayInstance);
		return _overlayInstance;
	};

	// future proofing
	var run = $('body').attr('data-ocrext-run');
	var textOverlay;
	if (run) {
		textOverlay = TextOverlay();
		textOverlay.listenToBackgroundEvents();
		textOverlay.setTitle();
	}
	if(!$('body').attr('data-ocrext-dialog')){
		window.__copyFishHtmlDialog__ = htmlDialog();
		window.__copyFishHtmlDialog__.init();
	}

	window.__TextOverlay__ = TextOverlay;
	
}());



================================================
FILE: scripts/screencapture.js
================================================
'use strict';

let dataUrls = [];

tabbis({
	tabActiveFallback: 0
});

document.addEventListener("tabbis", e => {
	let data = e.detail.tab;

	if ($(data).attr('id') === 'tab-0-0') {
		$('#tabpanel-0-0').removeClass('canvas-hidden')
	} else {
		$('#tabpanel-0-0').addClass('canvas-hidden')
	}
}, false);


let ScreenCap = {

	sum: (...list) => {
		return list.reduce((x, y) => x + y, 0);
	},

	blobToDataURL: (blob, withBase64Prefix = false) => {
		return new Promise((resolve, reject) => {
			let reader = new FileReader()
			reader.onerror = reject
			reader.onload = (e) => {

				const str = reader.result

				const b64 = 'base64,'
				const i = str.indexOf(b64)
				const ret = str.substr(i + b64.length)

				$('.copyfish-image-view').attr('src', 'data:image/png;base64, ' + ret)
				resolve(ret)
			}
			reader.readAsDataURL(blob)
		})
	},

	dataURItoArrayBuffer: (dataURI) => {
		// convert base64 to raw binary data held in a string
		// doesn't handle URLEncoded DataURIs - see SO answer #6850276 for code that does this
		var byteString = atob(
			/^data:/.test(dataURI) ? dataURI.split(',')[ 1 ] : dataURI
			);

		// write the bytes of the string to an ArrayBuffer
		var ab = new ArrayBuffer(byteString.length);

		// create a view into the buffer
		var ia = new Uint8Array(ab);

		// set the bytes of the buffer to the correct values
		for (var i = 0; i < byteString.length; i++) {
			ia[ i ] = byteString.charCodeAt(i);
		}

		return ab
	},

	concatUint8Array: (...arrays) => {
		const totalLength = ScreenCap.sum(...arrays.map(arr => arr.length));
		const result = new Uint8Array(totalLength);
		for (let i = 0, offset = 0, len = arrays.length; i < len; i += 1) {
			result.set(arrays[ i ], offset);
			offset += arrays[ i ].length;
		}
		return result;
	},

	readFileAsArrayBuffer: (range) => {

		return new Promise((resolve, reject) => {
			const result = range.rangeEnd > range.rangeStart ? dataUrls.concat([ range.buffer ]) : dataUrls;

			console.log(dataUrls, 12312312);
			const arr = ScreenCap.concatUint8Array(...result.map(result => new Uint8Array(ScreenCap.dataURItoArrayBuffer(result))));
			console.log(arr.buffer, 12312312);
			resolve(arr.buffer);
		});
	},

	readFileAsBlob: (range) => {
		return new Promise((resolve, reject) => {
			resolve(ScreenCap.readFileAsArrayBuffer(range)
				.then(buffer => new Blob([ buffer ])));
		});

	},

	readFileAsDataURL: (range, withBase64Prefix = true) => {

		return ScreenCap.readFileAsBlob(range)
		.then(blob => ScreenCap.blobToDataURL(blob, withBase64Prefix));
	},

	init: function () {
		this.title = browser.i18n.getMessage('appName') + ' - ' + browser.i18n.getMessage('screenCapture');
		$('title,.title').text(this.title);
		$('.placeholder').text(browser.i18n.getMessage('screenCaptureWaitMessage'));
		$('.settings-icon').click((e) => {
			e.stopPropagation();
			browser.runtime.sendMessage({
				evt: 'open-settings'
			});
		})

		function resizedataURL(datas) {
			var imageLoadDfd = $.Deferred();
			// We create an image to receive the Data URI
			var img = document.createElement('img');
			var devicePxRatio = devicePixelRatio;
			var scaleValue = 1 / devicePxRatio;
			// When the event "onload" is triggered we can resize the image.
			img.onload = function () {
				// We create a canvas and get its context.
				var canvas = document.createElement('canvas');
				var ctx = canvas.getContext('2d');
				var wantedWidth = img.width * scaleValue;
				var wantedHeight = img.height * scaleValue;
				// We set the dimensions at the wanted size.
				canvas.width = wantedWidth;
				canvas.height = wantedHeight;
				// We resize the image with the canvas method drawImage();
				ctx.drawImage(this, 0, 0, wantedWidth, wantedHeight);
				var dataURI = canvas.toDataURL();
				imageLoadDfd.resolve(dataURI);
				//return dataURI;
				/////////////////////////////////////////
				// Use and treat your Data URI here !! //
				/////////////////////////////////////////
			};
			img.src = datas;
			// We put the Data URI in the image's src attribute

			return imageLoadDfd;
		}
		function copyToClipboard(request, sendResponse) {
			let copyDivElm = document.createElement('div');
			copyDivElm.contentEditable = true;
			copyDivElm.style.opacity = 0;
			copyDivElm.style = "white-space:pre-wrap;"
			document.body.appendChild(copyDivElm);
			copyDivElm.textContent = request && request.data || '';
			copyDivElm.unselectable = 'off';
			copyDivElm.focus();
			document.execCommand('SelectAll');
			document.execCommand('Copy', false, null);
			document.body.removeChild(copyDivElm);
			request.onComplete && request.onComplete();
		}

		browser.runtime.onMessage.addListener(function (request, sender, sendResponse) {
			//console.log(request.evt, 8789)
			if (request.evt === 'desktopcaptureData') {
				console.log(request)

				if (request.ocrEngine == "OcrLocal") {

						// enable only if resources are loaded and available
						if (request.result.buffer) {
							resizedataURL(`data:application/octet-stream;base64,${request.result.buffer}`).done(function (resultDataUri) {
								request.result.buffer = resultDataUri;
								$('#copyfish-tab-image').attr('src', `${request.result.buffer}`)
								browser.runtime.sendMessage({ evt: "translateDesktopCapturedImage", data: `${request.result.buffer}`,imagepath: `${request.imagepath}`})
								const setImage = ScreenCap.readFileAsDataURL(request.result);
								$('.placeholder')
								.text(browser.i18n.getMessage('screenCaptureNotify'))
								.addClass('notify');
							});
						} else {
							$('#copyfish-tab-image').attr('src', request.result);
							browser.runtime.sendMessage({ 	evt: "translateDesktopCapturedImage",
								imagepath: request.imagepath,
								data: request.result,
								ocrText: request.ocrText || '',
								overlayInfo : request.overlayInfo || '',
								forExternalTab:request.forExternalTab || 0,
								translatedTextIfAny	: request.translatedTextIfAny || '',
								currentZoomLevel	: request.currentZoomLevel || 0,
							})
						}	

					}else{
				// enable only if resources are loaded and available
				if (request.result.buffer) {
					resizedataURL(`data:application/octet-stream;base64,${request.result.buffer}`).done(function (resultDataUri) {
						request.result.buffer = resultDataUri;
						$('#copyfish-tab-image').attr('src', `${request.result.buffer}`)
						browser.runtime.sendMessage({ evt: "translateDesktopCapturedImage", data: `${request.result.buffer}` })
						const setImage = ScreenCap.readFileAsDataURL(request.result);
						$('.placeholder')
						.text(browser.i18n.getMessage('screenCaptureNotify'))
						.addClass('notify');
					});
				} else {
					$('#copyfish-tab-image').attr('src', request.result);
					browser.runtime.sendMessage({ 	evt: "translateDesktopCapturedImage",
						imagepath: request.imagepath,
						data: request.result,
						ocrText: request.ocrText || '',
						overlayInfo : request.overlayInfo || '',
						forExternalTab:request.forExternalTab || 0,
						translatedTextIfAny	: request.translatedTextIfAny || '',
						currentZoomLevel	: request.currentZoomLevel || 0,
					})
				}	
			}
		} else if (request.evt === 'copyToClipboard') {
			copyToClipboard(request, sendResponse);
		}
			// ACK back
			return true;
		});
	}
};
$(function(){
	$(ScreenCap.init);
});





================================================
FILE: scripts/._background.js
================================================
[Binary file]


================================================
FILE: scripts/._options.js
================================================
[Binary file]


================================================
FILE: styles/cs.css
================================================
/*right*/

.ocrext-preserve-whitespace {
	white-space: pre;
}

.ocrext-ch {
	cursor: crosshair;
}

.ocrext-overlay {
	-webkit-user-select: none;
}

.ocrext-overlay .ocrext-mask {
	position: absolute;
	/*background: @black;
    opacity: 1;
    .transition-opacity();*/
	/*cursor: crosshair;
	transform:none;*/
	min-height: initial;
	min-width: intial;
	/* max-width: 100%;
	max-height: 100%; */
}

.ocrext-overlay .ocrext-mask .ocrext-overlay-corner {
	/*.transition-opacity();*/
	/*opacity: 0;*/
	background: #757575;
	cursor: crosshair;
	transform: none;
	position: absolute;
	background: rgba(117, 117, 117, 0.3);
	top: 0;
	left: 0;
	/*width: 100%;
	height: 100%;*/
}

.ocrext-overlay .ocrext-mask p.ocrext-element {
	font-size: 54px;
	top: 25%;
	color: #000;
	text-align: center;
	/*transform:scale(0,0);
	.transition-all();*/
	position: relative;
	/*text-shadow: 5px 3px 5px #DECECE;*/
}

.ocrext-overlay .ocrext-selector {
	background: none;
	position: absolute;
	border: 2px solid #ff5252;
	border-radius: 2px;
}

.ocrext-element {
	margin: 0;
	padding: 0;
	border: 0;
	font-size: 100%;
	vertical-align: baseline;
	font: inherit;
	line-height: normal;
	text-decoration: none;
	text-transform: none;
	color: #757575;
	font-family: 'Roboto', Helvetica, Arial, serif;
	font-size: 16px;
	font-variant: normal;
}

.canvas-hidden {
	display: none !important;
	position: absolute;
	top: -800px;
}

.copy-hidden{
	position: fixed;
	left: 50%;
	top: 50%;
	transform: translate(-50%, -50%);
}
#copyfish-tab-image-container {
	position: relative;
	min-width: 430px;
	max-height: 200px;
	width: 100%;
	box-shadow: rgba(0, 0, 0, 0.14) 0px 2px 2px 0px, rgba(0, 0, 0, 0.2) 0px 3px 1px -2px, rgba(0, 0, 0, 0.12) 0px 1px 5px 0px;
	margin: 5px auto 10px;
	overflow: auto;
	transition: height 0.25s ease 0s;
}

.ocrext-textoverlay-container {
	cursor: pointer;
	display: none;
	border: none;
	position: absolute;
	top: 0;
	width: 100%;
	height: 100%;
}

.tabs-container .ocrext-result {
	padding-right: 18px !important;
}

.text-capture-tab h5, .translate-text-tab h5 {
	font-size: 18px;
	color: #227fd2;
}

.text-capture-tab textarea, .translate-text-tab .ocrext-ocr-translated {
	margin-top: 14px !important;
	min-height: 120px !important;
	width: -moz-available !important;
	width: -webkit-fill-available !important;
	width: fill-available !important;
	padding: 10px !important;
	border: 2px solid #393939 !important;
}

[data-panes]>* {
	padding: 16px !important;
}

.tabs-container .overlay-tab .ocrext-textoverlay-container {
	width: auto !important;
	top: 94px !important;
}

.tabs-container .ocrext-close-link {
	display: none !important;
}

.ocrext-textoverlay-container.visible {
	display: block;
	margin: 15px 30px;
}

.tabs-container .ocrext-textoverlay-container.visible {
	margin: 0;
}

.ocrext-textoverlay-container .ocrext-close-link {
	display: inline-block;
	position: absolute;
	padding: 0;
	height: 24px;
	width: 24px;
	background: url('chrome-extension://__MSG_@@extension_id__/images/close.png');
	background-size: 24px;
	top: 0;
	right: 0;
	cursor: pointer;
}

.ocrext-textoverlay-container .ocrext-text-overlay {
	display: none;
	margin: 15px auto;
}

.ocrext-textoverlay-container .ocrext-text-overlay {
	margin: 0 !important;
}

.ocrext-textoverlay-container .ocrext-text-overlay.visible {
	display: flex;
	justify-content: space-around;
}

.ocrext-textoverlay-container .ocrext-text-overlay .ocrext-text-overlay-word-wrapper {
	position: relative;
	border: none;
}

.ocrext-textoverlay-container .ocrext-text-overlay .ocrext-text-overlay-word-wrapper .ocrext-text-overlay-word {
	color: #ff0000;
	text-align: center;
	font-size: 20px;
	font-weight: bold;
	position: absolute;
	background: rgba(255, 215, 15, 0.5);
}

.ocrext-wrapper {
	background: #ffffff;
	border: none;
	border-radius: 10px;
	box-shadow: 0 2px 2px 0 rgba(0, 0, 0, 0.14), 0 3px 1px -2px rgba(0, 0, 0, 0.2), 0 1px 5px 0 rgba(0, 0, 0, 0.12);
	-webkit-box-shadow: 0 2px 2px 0 rgba(0, 0, 0, 0.14), 0 3px 1px -2px rgba(0, 0, 0, 0.2), 0 1px 5px 0 rgba(0, 0, 0, 0.12);
	color: #757575;
	display: none;
	font-family: 'Roboto', Helvetica, Arial, serif;
	font-size: 16px;
	font-weight: normal;
	font-variant: normal;
	right: 2%;
	width: 65%;
	max-width: 1020px;
	min-height: 200px;
	min-width: 680px;
	position: fixed;
	/*.transition-bottom();*/
	transition: all 0.25 ease;
	-webkit-transition: all 0.25s ease;
	/* start commented backslash hack \*/
	/* close commented backslash hack */
	/*Header*/
	/*Content*/
	/* Minimized State*/
}

.ocrext-wrapper * {
	font: inherit;
	line-height: normal;
	color: inherit;
	text-transform: none;
}

.ocrext-wrapper .ocrext-clearfix:after {
	visibility: hidden;
	display: block;
	font-size: 0;
	content: " ";
	clear: both;
	height: 0;
}

.ocrext-wrapper .ocrext-clearfix {
	display: inline-block;
}

.ocrext-wrapper * html .ocrext-clearfix {
	height: 1%;
}

.ocrext-wrapper .ocrext-clearfix {
	display: block;
}

.ocrext-wrapper header.ocrext-header {
	background: #2196f3;
	border-radius: 10px 10px 0 0;
	position: relative;
	left: 0;
	top: 0;
	height: auto;
	padding: 0;
	margin: 0;
	min-width: initial;
	min-height: 45px;
	height: 45px;
	box-sizing: border-box;
	overflow: hidden;
	box-shadow: 0 2px 2px 0 rgba(0, 0, 0, 0.14), 0 3px 1px -2px rgba(0, 0, 0, 0.2), 0 1px 5px 0 rgba(0, 0, 0, 0.12);
	-webkit-box-shadow: 0 2px 2px 0 rgba(0, 0, 0, 0.14), 0 3px 1px -2px rgba(0, 0, 0, 0.2), 0 1px 5px 0 rgba(0, 0, 0, 0.12);
	cursor: pointer;
}

.ocrext-wrapper header.ocrext-header .ocrext-title {
	color: #ffffff;
	text-align: left;
	/*padding: @header-padding; */
	border-radius: 10px 10px 0 0;
	font-weight: bold;
	font-size: 20px;
	padding: 15px 0 8px 15px;
	margin: 0;
}

.ocrext-wrapper header.ocrext-header .ocrext-title .ocrext-settings-link {
	display: inline-block;
	float: right;
	margin: -4px 25px 0 0;
	padding: 0;
	height: 24px;
	width: 24px;
	/*background: url('chrome-extension://__MSG_@@extension_id__/images/gear.png');*/
}

.tabs-container .ocrext-btn {
	padding: 10px 20px;
	margin-right: 16px;
}

.tabs-container .ocrext-footer {
	display: flex;
	align-items: center;
	justify-content: flex-end;
}

.tabs-container .ocrext-btn-container {
	display: flex;
	width: auto;
	margin-right: 60px;
}

.tabs-container .ocrext-quickselect-btn-container {
	height: 45px;
	width: auto;
	margin-right: 60px;
}

.ocrext-wrapper header.ocrext-header .ocrext-title .ocrext-open-tab-link {
	display: inline-block;
	float: right;
	margin: -4px 25px 0 0;
	padding: 0;
	height: 24px;
	width: 24px;
	background-size: 20px !important;
	background-repeat: no-repeat !important;
	background-position: center !important;
	/*background: url('chrome-extension://__MSG_@@extension_id__/images/outside.png');*/
}

.ocrext-wrapper header.ocrext-header .ocrext-title .ocrext-closeToolbar-link {
	display: inline-block;
	float: right;
	margin: -4px 25px 0 0;
	padding: 0;
	height: 24px;
	width: 24px;
	/*background: url('chrome-extension://__MSG_@@extension_id__/images/close.png');*/
}

.ocrext-wrapper header.ocrext-header .ocrext-title .ocrext-closeToolbar-link.firefox-ocrext {
	background: url('moz-extension://__MSG_@@extension_id__/images/close.png');
}

.ocrext-wrapper header.ocrext-header .ocrext-title .ocrext-open-tab-link.firefox-ocrext {
	background: url('moz-extension://__MSG_@@extension_id__/images/outside.png');
}

.ocrext-wrapper header.ocrext-header .ocrext-title .ocrext-settings-link.firefox-ocrext {
	background: url('moz-extension://__MSG_@@extension_id__/images/gear.png');
}

.ocrext-wrapper .ocrext-content {
	padding: 5px 30px 10px 30px;
	/*Canvas container*/
	/*Ouput Container*/
	/*Bottom buttons container*/
	/*Output disabled*/
	/*Output Error*/
}

.ocrext-wrapper .ocrext-content .ocrext-canvas-container {
	position: relative;
	margin: 5px auto 10px auto;
	min-width: 430px;
	max-height: 200px;
	width: 100%;
	overflow: auto;
	box-shadow: 0 2px 2px 0 rgba(0, 0, 0, 0.14), 0 3px 1px -2px rgba(0, 0, 0, 0.2), 0 1px 5px 0 rgba(0, 0, 0, 0.12);
	-webkit-box-shadow: 0 2px 2px 0 rgba(0, 0, 0, 0.14), 0 3px 1px -2px rgba(0, 0, 0, 0.2), 0 1px 5px 0 rgba(0, 0, 0, 0.12);
	transition: height 0.25 ease;
	-webkit-transition: height 0.25s ease;
}

.ocrext-wrapper .ocrext-content .ocrext-canvas-container #ocrext-can {
	margin: 15px 30px;
	display: block;
	cursor: pointer;
}

.ocrext-wrapper .ocrext-content .ocrext-ocr-output {
	/*margin:30px 0;*/
	padding: 0 7px;
	/* max-height: @output-max-height;
	min-height: @output-min-height;  */
	height: 200px;
	margin-bottom: 10px;
	overflow-y: hidden;
	position: relative;
	transition: height 0.25 ease;
	-webkit-transition: height 0.25s ease;
	/* p{
        margin:10px 0 0 0;
        text-align: left;
        } */
        /*padding:5px;*/
    }

    .ocrext-wrapper .ocrext-content .ocrext-ocr-output .ocrext-status {
    	text-align: center;
    	padding: 0 10px;
    	margin: 0 7px 5px 0;
    	height: 0px;
    	overflow: hidden;
    	font-weight: bold;
    	font-size: 14px;
    	transition: height 0.25 ease;
    	-webkit-transition: height 0.25s ease;
    }

    .ocrext-wrapper .ocrext-content .ocrext-ocr-output .ocrext-status.ocrext-error {
    	color: #d9534f;
    	height: 16px;
    }

    .ocrext-wrapper .ocrext-content .ocrext-ocr-output .ocrext-status.ocrext-success {
    	color: #8bc34a;
    	height: 16px;
    }

    .ocrext-wrapper .ocrext-content .ocrext-ocr-output .ocrext-status.ocrext-progress {
    	color: #03a9f4;
    	height: 16px;
    }

    .ocrext-wrapper .ocrext-content .ocrext-ocr-output .ocrext-spinner-wrapper {
    	position: absolute;
    	left: 50%;
    	top: 60px;
    	pointer-events: none;
    }

    .ocrext-wrapper .ocrext-content .ocrext-ocr-output .ocrext-spinner-wrapper .ocrext-spinner {
    	width: 80px;
    	height: 80px;
    	position: relative;
    	left: -50%;
    }

    .ocrext-wrapper .ocrext-content .ocrext-ocr-output .ocrext-grid {
    	padding: 0 8px;
    	box-sizing: border-box;
    }

    .ocrext-wrapper .ocrext-content .ocrext-ocr-output .ocrext-grid:last-child {
    	margin: 14px auto 0 auto;
    }

    .ocrext-wrapper .ocrext-content .ocrext-ocr-output .ocrext-cell {
    	float: left;
    	margin: 0;
	/* &:nth-child(2) {
          margin-left: 25px;
          } */
      }

      .ocrext-wrapper .ocrext-content .ocrext-ocr-output .ocrext-cell.ocrext-cell-small {
      	width: 20%;
      }

      .ocrext-wrapper .ocrext-content .ocrext-ocr-output .ocrext-cell.ocrext-cell-large {
      	width: 80%;
      }

      .ocrext-wrapper .ocrext-content .ocrext-ocr-output h5.ocrext-label {
      	/*margin:0 0 5px 0;*/
      	border-right: 2px solid #2196f3;
      	padding: 8px 10px 3px 0;
      	font-weight: bold;
      	font-size: 16px;
      	word-wrap: break-word;
      	text-align: center;
      }

      .ocrext-wrapper .ocrext-content .ocrext-ocr-output h5.ocrext-label span {
      	font: inherit;
      	font-size: 14px;
      	line-height: 1.5;
      	display: inline-block;
      	width: 100%;
      	padding: 2px 2px 0 0;
      	overflow: hidden;
      	text-overflow: ellipsis;
      	white-space: nowrap;
      	word-wrap: normal;
      }

      .ocrext-wrapper .ocrext-content .ocrext-ocr-output p.ocrext-result {
      	/*margin:10px 0 15px 0;*/
      	box-sizing: border-box;
      	padding: 5px 0 0 15px;
      	height: 85px;
      	overflow-y: auto;
      	-webkit-user-select: text;
      	user-select: text;
      	text-align: justify;
      	font-size: 16px;
      	line-height: 22px;
      	word-wrap: break-word;
      }

      .ocrext-wrapper .ocrext-content .ocrext-ocr-output textarea.ocrext-result {
      	width: 100%;
      	height: 85px;
      	margin: 5px 0 0 15px;
      	resize: none;
      }

      .ocrext-wrapper .ocrext-content .ocrext-ocr-output .ocrext-ocr-message.ocrext-font-48px, .ocrext-wrapper .ocrext-content .ocrext-ocr-output .ocrext-ocr-translated.ocrext-font-48px {
      	font-size: 48px;
      	line-height: 48px;
      }

      .ocrext-wrapper .ocrext-content .ocrext-ocr-output .ocrext-ocr-message.ocrext-font-42px, .ocrext-wrapper .ocrext-content .ocrext-ocr-output .ocrext-ocr-translated.ocrext-font-42px {
      	font-size: 42px;
      	line-height: 42px;
      }

      .ocrext-wrapper .ocrext-content .ocrext-ocr-output .ocrext-ocr-message.ocrext-font-36px, .ocrext-wrapper .ocrext-content .ocrext-ocr-output .ocrext-ocr-translated.ocrext-font-36px {
      	font-size: 36px;
      	line-height: 36px;
      }

      .ocrext-wrapper .ocrext-content .ocrext-ocr-output .ocrext-ocr-message.ocrext-font-32px, .ocrext-wrapper .ocrext-content .ocrext-ocr-output .ocrext-ocr-translated.ocrext-font-32px {
      	font-size: 32px;
      	line-height: 32px;
      }

      .ocrext-wrapper .ocrext-content .ocrext-ocr-output .ocrext-ocr-message.ocrext-font-28px, .ocrext-wrapper .ocrext-content .ocrext-ocr-output .ocrext-ocr-translated.ocrext-font-28px {
      	font-size: 28px;
      	line-height: 28px;
      }

      .ocrext-wrapper .ocrext-content .ocrext-ocr-output .ocrext-ocr-message.ocrext-font-24px, .ocrext-wrapper .ocrext-content .ocrext-ocr-output .ocrext-ocr-translated.ocrext-font-24px {
      	font-size: 24px;
      	line-height: 24px;
      }

      .ocrext-wrapper .ocrext-content .ocrext-ocr-output .ocrext-ocr-message.ocrext-font-20px, .ocrext-wrapper .ocrext-content .ocrext-ocr-output .ocrext-ocr-translated.ocrext-font-20px {
      	font-size: 20px;
      	line-height: 24px;
      }

      .ocrext-wrapper .ocrext-content .ocrext-ocr-output .ocrext-ocr-message.ocrext-font-18px, .ocrext-wrapper .ocrext-content .ocrext-ocr-output .ocrext-ocr-translated.ocrext-font-18px {
      	font-size: 18px;
      	line-height: 22px;
      }

      .ocrext-wrapper .ocrext-content .ocrext-ocr-output .ocrext-ocr-message.ocrext-font-16px, .ocrext-wrapper .ocrext-content .ocrext-ocr-output .ocrext-ocr-translated.ocrext-font-16px {
      	font-size: 16px;
      	line-height: 20px;
      }

      .ocrext-wrapper .ocrext-content .ocrext-ocr-output .ocrext-ocr-message.ocrext-font-14px, .ocrext-wrapper .ocrext-content .ocrext-ocr-output .ocrext-ocr-translated.ocrext-font-14px {
      	font-size: 14px;
      	line-height: 18px;
      }

      .ocrext-wrapper .ocrext-content .ocrext-ocr-output .ocrext-ocr-message.ocrext-font-12px, .ocrext-wrapper .ocrext-content .ocrext-ocr-output .ocrext-ocr-translated.ocrext-font-12px {
      	font-size: 12px;
      	line-height: 16px;
      }

      .ocrext-wrapper .ocrext-content .ocrext-ocr-output .ocrext-ocr-message.expanded {
      	height: 170px;
      }

      .ocrext-wrapper .ocrext-content .ocrext-footer .ocrext-quickselect-btn-container {
      	/*float: left;*/
      	display: flex;
      	flex-direction: row;
      	justify-content: center;
      }

      .ocrext-wrapper .ocrext-content .ocrext-footer .ocrext-quickselect-btn-container button.ocrext-btn {
      	margin-bottom: 5px;
      	margin-right: 5px;
      	padding: 0 8px;
      	min-width: 46px;
      }

      .ocrext-wrapper .ocrext-content .ocrext-footer .ocrext-quickselect-btn-container button.ocrext-btn.selected {
      	cursor: default;
      	opacity: 0.6;
      }

      .ocrext-wrapper .ocrext-content .ocrext-footer .ocrext-quickselect-btn-container button.ocrext-btn:last-child {
      	margin-right: 0;
      }

      .ocrext-wrapper .ocrext-content .ocrext-footer .ocrext-btn-container {
      	/*float: right;*/
      	display: flex;
      	flex-direction: row;
      	justify-content: space-around;
	/* .ocrext-ocr-close {
          margin-right: 0;
          } */
      }

      .ocrext-wrapper .ocrext-content .ocrext-footer .ocrext-btn-container button.ocrext-btn {
      	margin-bottom: 5px;
      	/*margin-right:15px;*/
      	padding: 0 16px;
      }

      .ocrext-wrapper .ocrext-content.ocrext-disabled .ocrext-canvas-container #ocrext-can {
      	cursor: default;
      }

      .ocrext-wrapper .ocrext-content.ocrext-error .ocrext-ocr-output .ocrext-ocr-message, .ocrext-wrapper .ocrext-content.ocrext-error .ocrext-ocr-output .ocrext-message {
      	color: #d9534f;
      }

      .ocrext-wrapper.ocrext-wrapper-minimized .ocrext-content .ocrext-canvas-container {
      	height: 0px;
      	overflow: hidden;
      }

      .ocrext-wrapper.ocrext-wrapper-minimized .ocrext-content .ocrext-ocr-output {
      	height: 100px;
      	overflow-y: auto;
      }

      .ocrext-wrapper.ocrext-wrapper-minimized .ocrext-content .ocrext-ocr-output p.ocrext-ocr-message.expanded {
      	height: auto;
      }

      @media screen and (max-width: 960px) {
      	.ocrext-wrapper {
      		font-size: 16px;
      	}
      	.ocrext-wrapper .ocrext-content .ocrext-ocr-output .mdl-grid .mdl-cell--2-col {
      		max-width: 120px;
      	}
      	.ocrext-wrapper .ocrext-content .ocrext-ocr-output .mdl-grid .mdl-cell--10-col {
      		max-width: 420px;
      	}
      	.ocrext-wrapper .ocrext-content .ocrext-ocr-output h5.ocrext-label span {
      		max-width: 70px;
      	}
      	.ocrext-wrapper .ocrext-content .ocrext-footer .ocrext-quickselect-btn-container {
      		justify-content: flex-start;
      	}
      	.ocrext-wrapper .ocrext-content .ocrext-footer .ocrext-btn-container {
      		/*float: none;*/
		/* button.ocrext-btn {
            margin-right:10px;
            padding: 0 8px;
            } */
        }
    }

    body.ocrext-textoverlay-page {
    	font-family: 'Roboto', Helvetica, Arial, serif;
    	padding: 10px;
    	color: #757575;
    }

    body.ocrext-textoverlay-page .ocrext-textoverlay-title {
    	width: 960px;
    	text-align: center;
    	margin: 10px auto 10px auto;
    	padding-bottom: 20px;
    	border-bottom: 5px solid #2196f3;
    }

    body.ocrext-textoverlay-page .ocrext-textoverlay-container {
    	cursor: default;
    	position: relative;
    	width: 960px;
    	margin: 0 auto;
    	margin-top: 100px;
    }

    body.ocrext-textoverlay-page .ocrext-textoverlay-container a.ocrext-close-link {
    	display: none;
    }

    #popup_translate_button img {
    	height: 40px;
    	width: 42px;
    	cursor: pointer;
    }

    #deepl_translate_button img {
    	height: 40px;
    	width: 42px;
    	cursor: pointer;
    }
    #popup_translate_button svg,#deepl_translate_button svg{
    	cursor: pointer;
    }
    .translate-button {
    	margin-left: 8px;
    }

    /*Custom dialog popup*/

    /* Popup Open button */ 
    .cp-dialog-open-button{
    	color:#FFF;
    	background:#0066CC;
    	padding:10px;
    	text-decoration:none;
    	border:1px solid #0157ad;
    	border-radius:3px;
    }

    .cp-dialog-open-button:hover{
    	background:#01478e;
    }

    .cp-dialog-popup {
    	position:fixed;
    	top:0px;
    	left:0px;
    	background:rgba(0,0,0,0);
    	width:100%;
    	height:100%;
    	display:none;
    }

    /* Popup inner div */
    .cp-dialog-popup-content {
    	width: 30%;
    	margin: 0 auto;
    	box-sizing: border-box;
    	padding: 40px;
    	margin-top: 100px;
    	box-shadow: 0px 2px 6px rgba(0,0,0,1);
    	border-radius: 3px;
    	background: #fff;
    	position: relative;
    }

    /* Popup close button */
    .cp-dialog-close-button {
    	width: 25px;
    	height: 25px;
    	position: absolute;
    	top: -10px;
    	right: -10px;
    	border-radius: 20px;
    	background: rgba(0,0,0,0.8);
    	font-size: 20px;
    	text-align: center;
    	color: #fff;
    	text-decoration:none;
    }

    .cp-dialog-close-button:hover {
    	background: rgba(0,0,0,1);
    }

    #cfish-popup-message-dialog{z-index: 999; font-family: Roboto, Helvetica, Arial, sans-serif;color:#757575}
    #cfish-popup-message-dialog .cp-dialog-popup-content{padding: 0;}
    #cfish-popup-message-dialog .title-con{display: flex; align-items: center; padding: 10px 15px; background:#2196f3;}
    #cfish-popup-message-dialog .title-con img {width: 25px; height: 25px; object-fit: cover;}
    #cfish-popup-message-dialog #cp-dialog-title{color: #fff; padding: 0; margin: 0 0 0 10px;}
    #cfish-popup-message-dialog #cp-dialog-description{padding: 10px 15px 20px; font-size: 18px; line-height: 25px;}
    #cfish-popup-message-dialog #cp-dialog-description .button-row{padding-top: 25px;}
    #cfish-popup-message-dialog #cp-dialog-description .button-row.btn-center{text-align: center;}
    #cfish-popup-message-dialog #cp-dialog-description span{display: inline-block; margin-right: 10px; margin-bottom: 10px;}
    #cfish-popup-message-dialog #cp-dialog-description span:hover{opacity: 0.8;}
    #cfish-popup-message-dialog .cp-dialog-close-button{top: 7px; background: none; right: 5px;}
    #cfish-popup-message-dialog h2{ font-size: 1.5em;
    	font-weight: bolder;
    }
    @media screen and (max-width: 720px) {
    	.cp-dialog-popup-content {
    		width:90%;
    	} 
    }




================================================
FILE: styles/desktop.screencapture.css
================================================
.ocrext-textoverlay-container.visible{
    margin: 0 !important;
}

/* #tabpanel-0-1 .ocrext-textoverlay-container{
    top: 92px !important;
} */

.ocrext-element.ocrext-status{
    display: flex;
    align-items: center;
    justify-content: center;
    opacity: 0;
}

.ocrext-progress {
    color: #03a9f4;
    height: 16px;
    opacity: 1 !important;
}

.ocrext-success {
    color: #8bc34a;
    height: 16px;
    opacity: 1 !important;
}

.ocrext-error {
    color: #d9534f;
    height: 16px;
    opacity: 1 !important;
}

.ocrext-footer{
    padding-top: 0 !important;
}

.copyfish-tab-text-lng h5{
    display: flex;
    flex-direction: column;
    justify-content: center;
}

.copyfish-text-translate{
    display: none;
}



================================================
FILE: styles/options.css
================================================
/*right*/

.ocrext-preserve-whitespace {
	white-space: pre;
}

body {
	font-size: 16px;
	color: #757575;
	padding: 0;
	margin: 0;
}

.container {
	width: 960px;
	margin: 0 auto;
	padding: 10px 20px 20px 20px;
	box-sizing: border-box;
}

.container h1 {
	text-align: center;
	margin: 10px 0 10px 0;
	padding-bottom: 20px;
	border-bottom: 4px solid #2196f3;
}

.container .inner-wrapper {
	margin: 25px 0 0 0;
}

.container .inner-wrapper .status-text {
	color: #2196f3;
	width: 100%;
	text-align: center;
	transition: opacity 0.6s;
	opacity: 0;
	padding-left: 45px;
	margin-top: 0;
	font-size: 18px;
}

.container .inner-wrapper .status-text.visible {
	opacity: 1;
}

.container .inner-wrapper .subtitle h2 {
	margin: 0 auto;
	color: #000000;
}

.container .inner-wrapper .options-container {
	margin-top: 5px;
}

.container .inner-wrapper .options-container:nth-child(1) {
	margin-top: 0px;
}

.container .inner-wrapper .label {
	padding: 8px 20px 0 0;
	text-align: right;
}

.container .inner-wrapper .label label a.tip {
	font-size: 14px;
	color: #2196f3;
	display: inline-block;
	width: 170px;
}

.container .inner-wrapper select {
	width: 400px;
	padding: 8px 4px;
	font-size: 16px;
}

.container .inner-wrapper select.hotkeySelector {
	display: inline-block;
	width: 80px;
	font-size: 16px;
}

.container .inner-wrapper select.lang-quickselect {
	width: 120px;
}

.container .inner-wrapper label.auto-process,
.container .inner-wrapper label.auto-translate,
.container .inner-wrapper label.popup-dicts {
	margin-top: 8px;
}

.container .inner-wrapper .subscription {
	max-width: 500px;
	border: 2px #F5CE62 solid;
	margin: 0 auto;
	padding: 15px 20px;
}

.container .inner-wrapper .subscription .text {
	font-size: 20px;
	color: #7B77F7;
	text-align: center;
	margin: 10px auto;
}

.container .inner-wrapper .subscription label {
	display: block;
	color: #000000;
	margin-bottom: 10px;
}

.container .inner-wrapper .subscription input[type='email'] {
	display: block;
	width: 100%;
	margin-bottom: 20px;
}

.container .inner-wrapper .subscription input[type='submit'] {
	font-size: 13px;
	border: none;
	-moz-border-radius: 3px;
	border-radius: 3px;
	letter-spacing: .03em;
	color: #fff;
	background-color: #aaa;
	box-sizing: border-box;
	height: 32px;
	line-height: 32px;
	padding: 0 18px;
	display: block;
	margin: 0;
	transition: all 0.23s ease-in-out 0s;
}

.container .inner-wrapper .subscription input[type='submit']:hover {
	background-color: #777;
	cursor: pointer;
}

.container .inner-wrapper .btn {
	padding: 0 20px;
}

.container .inner-wrapper .btn:nth-child(2) {
	margin-left: 15px;
}

.container .inner-wrapper .extension-link {
	color: #2196f3;
	font-size: 16px;
}

.container .inner-wrapper .buttons .button-cell {
	text-align: center;
}
@media screen and (max-width: 700px) {
	.OcrEngine label, .TranslationEngine label, .copy_options label{
		margin-left: 50px !important;
	}
}
@media screen and (max-width: 959px) {

	.container {
		width: 100%;
		margin: 0 auto;
	}
	.container .inner-wrapper .label {
		text-align: left;
	}
	.container .inner-wrapper select {
		width: 90%;
	}
	.EnginesType{
		width: 95% !important;
		text-align: left !important;
		margin: 0 0 15px 10px !important;
	}
}
.OcrEngine,.TranslationEngine, .copy_options{
	width: 100%;
	display: grid;
  grid-template-columns: 1fr;
  grid-template-rows: 1fr;
  grid-template-areas: "." ".";
	align-items: center;
  justify-items: center;
	margin-bottom: 10px;
	/* width: 100%;
	display: flex;
	justify-content: center;
 flex-direction: column;
	align-items: center;
	margin-bottom: 15px; */
}
.OcrEngine label,.TranslationEngine label, .copy_options label{
	display: run-in !important;
	width: 400px;
	margin-left: 274px;
	margin-bottom: 5px;

}

.EnginesType{
	margin: 0 auto -20px 0px;
	width: 38%;
	text-align: right;
	display:block;

}

.upgrade_status{
  transition-duration: 0.4s;
	margin-left: 20px;
	box-shadow: none;
	outline: none;
	color: #555;
	cursor: pointer;
	-webkit-border-radius: 10px;
	-moz-border-radius: 10px;
	border-radius: 10px;
	height: 35px;
	width: 150px;
	font-size: 15px;
}
.upgrade_status:hover{
	color:white;
	background-color:#777;
}

.show_status{
	font-weight: 600;
	color: #527700;
}
.ProPlans{
display: flex;
flex-direction: column;
justify-content: center;
align-items: center;
}
.file-access-status{
	display: flex;
	flex-direction: column;
	justify-content: center;
	align-items: center;

}

.file-access-status h2{
	color: #000;
}
.file-access-main{
		width: 60%;
}
.file-access-main div{
		display: none;
}

/* file-access-error styles */
.file-access-main .file-access-status-error{
	color: red;
	margin-bottom: 10px;
}
.file-access-main .file-access-status-error a{
	color: #0000EE;
}

/* file-access-done styles */
.file-access-main .file-access-status-done{
	color: green;
	margin-bottom: 10px;
}
.ProPlans_text, .shortcut-options-info{
	color: #5f5c5c;
	width: 55%;
	margin-bottom: 15px;
	margin-top: 14px;
}
.shortcut-options-info {
	font-size: 13px;
	width: 80%;
	display: flex;
	align-items: center;
	justify-content: center;
}

.shortcut-options-info a{
	color: gray;
}
.shortcut-options{
	align-items: center;
	justify-content: center;
}

.margin-top{
	margin-top: 12px;
}

.copy-auto-container{
	display: flex;
	flex-direction: column;
	justify-content: center;
	align-items: center
}
.copy-auto-container h2{
	color: #000;
}
.copy-auto-text{
	width: 100%;
}


.ProPlans h2{
	color: black;
}
.keyChecker{
	margin-bottom: 10px;
}
.keyChecker button{
	font-size: 13px;
	margin-left: 5px;

	-moz-border-radius: 3px;
	border-radius: 3px;
	line-height: 24px ;
	height: 25px;
	width: 100px;
}
.keyChecker button:hover{
	color:white;
	background-color:#777;
}

#status_msg{
	color: red;
}
#status_msg_success{
	color: #2d7f17;
}

.currentStatus{
	margin-bottom: 15px;
}

.xmodule-item{
	display: flex;
	justify-content: space-between;
	align-items: center;
	flex-direction: column;
	margin: 40px 0;
}

.xmodule-status{
	margin: 10px 0;
	width: 68%;
	display: flex;
	justify-content: flex-start;
	align-items: center;
	flex-wrap: wrap;
}

#xmodule-shutter {
	width: 100%;
	display: none;
	color: red;
}

.xmodule-title {
	width: 68%;
	display: flex;
	justify-content: space-between;
	align-items: center;
}

#check-update-xmodule, #check-update-fileaccess{
	position: relative;
	bottom: -15px;
}

.xmodule-title b {
	color: #595959;
}

.xmodule-title a,.xmodule-status a {
	color: #2b9beb;
	text-decoration: none;
}

.disabled-background{
	background: #dddddd;
}
.xmodule-item button{
	width: 80px;
	height: 35px;
	border: none;
	text-decoration: none;
	border-radius: 8px;
	color: #fff;
	background-color: #108ee9;
	cursor: pointer;

}
.xmodule-item button:hover {
	opacity: 0.7;
}
.xmodule-title {
	margin-top: 20px;
}
.options-link{
	color: #2b9beb;
	text-decoration: none;
}
.container {
	font-family: 'Roboto', Helvetica, Arial, serif;
	font-variant: normal;
}
.disabled-trans-block{background-color:#eeebeb;}


================================================
FILE: styles/reset.css
================================================
/* http://meyerweb.com/eric/tools/css/reset/ 
   v2.0 | 20110126
   License: none (public domain)
*/

html, body, div, span, applet, object, iframe,
h1, h2, h3, h4, h5, h6, p, blockquote, pre,
a, abbr, acronym, address, big, cite, code,
del, dfn, em, img, ins, kbd, q, s, samp,
small, strike, strong, sub, sup, tt, var,
b, u, i, center,
dl, dt, dd, ol, ul, li,
fieldset, form, label, legend,
table, caption, tbody, tfoot, thead, tr, th, td,
article, aside, canvas, details, embed, 
figure, figcaption, footer, header, hgroup, 
menu, nav, output, ruby, section, summary,
time, mark, audio, video {
    margin: 0;
    padding: 0;
    border: 0;
    font-size: 100%;
    font: inherit;
    vertical-align: baseline;
}
/* HTML5 display-role reset for older browsers */
article, aside, details, figcaption, figure, 
footer, header, hgroup, menu, nav, section {
    display: block;
}
body {
    line-height: 1;
}
ol, ul {
    list-style: none;
}
blockquote, q {
    quotes: none;
}
blockquote:before, blockquote:after,
q:before, q:after {
    content: '';
    content: none;
}
table {
    border-collapse: collapse;
    border-spacing: 0;
}


================================================
FILE: styles/screencapture.css
================================================
html, body {
	margin: 0;
	padding: 0;
	height: 100%;
}

[data-tabs]>[role=tab] {
	background: rgb(33, 150, 243);
	color: #fff !important;
	cursor: pointer;
}

#copyfish-tab-image-container {
	width: 100%;
	box-shadow: -3px 3px 15px 3px rgba(0, 0, 0, 0.1);
	min-height: none !important;
	max-height: none !important;
	overflow: auto;
	margin-bottom: 20px;
	height:50%
}
.ocrext-element.ocrext-status{
	height:auto;
}
.ocrext-grid-translated, .copyfish-tab-text{
	height:33%;
}
.ocrext-canvas-container {
	width: 100%;
	overflow: auto;
	position: relative;
}

.mdl-button--accent.mdl-button--accent.mdl-button--raised, .mdl-button--accent.mdl-button--accent.mdl-button--fab {
	padding: 10px 20px;
	margin: 0 10px;
}

.ocrext-btn-container {
	display: flex !important;
}

.copyfish-tab-text {
	width: 100%;
	display: flex;
	align-items: flex-start;
	justify-content: space-between;
	flex-direction: row;
}

.copyfish-tab-text .ocrext-ocr-translated, .copyfish-tab-text .ocrext-element.ocrext-cell.ocrext-cell-large {
	width: 100% !important;
}

.translate-text-tab.ocrext-grid-translated.disabled {
	display: block !important;
	color: #757575;
	background-color: #9b9b9b;
	pointer-events: none !important;
}

.settings-icon {
	width: 32px;
	cursor: pointer;
	position: relative;
}

.settings-icon-container {
	width: calc(100% - 20px);
	height: 0;
	position: relative;
	display: flex;
	align-items: flex-end;
	justify-content: flex-end;
	top: -3px;
}

.copyfish-tab-text textarea, .copyfish-tab-text p.ocrext-ocr-translated {
	min-width: 80%;
	max-width: 80%;
	color: rgb(117, 117, 117);
	font-size: 16px;
	font-weight: normal;
	border: none;
	height: 100%;
	line-height: 20px;
	padding: 5px 0px 0px 15px;
}

.ocrext-ocr-translated {
	overflow: auto !important;
	min-width: calc(100% - 15px) !important;
}

.copyfish-tab-text {
	margin-top: 12px;
}

.text-capture-tab textarea, .text-capture-tab textarea, .translate-text-tab .ocrext-ocr-translated {
	color: rgb(117, 117, 117) !important;
	font-size: 16px !important;
	font-weight: normal !important;
}

.copyfish-capture-lng {
	margin-top: 5px;
}

.copyfish-tab-text-lng {
	height: 50px;
	display: flex;
	align-items: center;
	flex-direction: column;
	justify-content: center;
	width: 22%;
	padding: 8px 10px 3px 0px;
	font-weight: bold;
	font-size: 16px;
	overflow-wrap: break-word;
	text-align: center;
	color: rgb(117, 117, 117);
	border-right: 2px solid rgb(33, 150, 243);
}

[data-tabs] {
	/*width: 478px;*/
	margin-top: 30px;
	border: 2px solid rgb(9, 144, 246);
	border-top-left-radius: 3px;
	border-bottom: none;
}

[data-tabs] * {
	transition: background-color 0.2s ease !important;
}

[data-panes]>:focus, [data-tabs]>[role=tab]:focus {
	border: none !important;
}

[data-tabs]>[role=tab][aria-selected=true] {
	border: none !important;
}

[data-tabs] {
	position: relative;
	z-index: 2;
	top: 2px;
}

.tabs-container {
	width: 80%;
}

[data-tabs]>[role=tab]:hover {
	background: rgb(23, 153, 250) !important;
	color: #fff;
}

[data-panes] {
	border: 2px solid rgb(9, 144, 246) !important;
	border-top-right-radius: 3px;
	border-bottom-right-radius: 3px;
	border-bottom-left-radius: 3px;
}

[data-tabs]>[aria-selected=true] {
	color: rgb(9, 144, 246) !important;
}

[data-tabs]>[aria-selected=true]:hover {
	background: #fff !important;
	color: rgb(9, 144, 246) !important;
}

.data-tab-block {
	float: left;
}

/*right*/

.ocrext-preserve-whitespace {
	white-space: pre;
}

body * {
	font-family: 'Roboto', sans-serif !important;
}

body {
	display: flex;
	justify-content: center;
	align-items: center;
	flex-direction: column;
}

.container {
	color: #757575;
	font-family: 'Roboto', Helvetica, Arial, serif;
	font-size: 16px;
	font-variant: normal;
}

.container h1.title {
	text-align: center;
	margin-bottom: 10px;
	padding: 15px;
	border-bottom: 4px solid #2196f3;
	width: 60%;
	min-width: 200px;
	margin-left: auto;
	margin-right: auto;
	font-size: 20px;
}

.container p.placeholder {
	font-size: 18px;
	padding: 10px;
	margin: 0 auto;
	text-align: center;
}

.container p.placeholder.notify {
	color: #ff5252;
}

.canvas-wrapper {
	display: flex;
	justify-content: center;
	flex-direction: column;
	align-items: center;
}

#slider {
	display: block;
	width: 50%;
	margin: 40px 0;
}

#slider #custom-handle {
	width: 40px;
	height: 25px;
	display: flex;
	justify-content: center;
	align-items: center;
	text-align: center;
}

.zoom-container button {
	z-index: 1000;
	margin-left: 20px;
	padding: 0 20px;
	height: 40px;
	line-height: 40px;
	border: 1px solid #fff;
	border-radius: 4px;
	font-size: 14px;
	cursor: pointer;
	-webkit-transition: all .3s ease;
	transition: all .3s ease;
	color: #fefefe;
	background: rgba(0, 123, 255, .75);
	position: absolute;
	left: 3px;
	top: 3px;
}

.zoom-container button:hover {
	background: rgba(0, 123, 255, 1);
}

#imageViewerContainer {
	background: url(data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQAgMAAABinRfyAAAADFBMVEUAAABaWlrMzMz////nPAkwAAAAAWJLR0QAiAUdSAAAAAlwSFlzAAAOxAAADsQBlSsOGwAAAAd0SU1FB+IDGRUHMxeV5KYAAAAXSURBVAjXY1i16v9/BiKI//9XrSKCAABNyDUhZP4pqwAAAABJRU5ErkJggg==);
}

.align-flex-center {
	align-items: center;
	justify-content: center;
}

.button-screencapture {
	width: 79.7%;
	border: solid rgb(9, 144, 246);
	border-width: 0 2px 2px 2px;
	padding-bottom: 15px;
	margin-bottom: 12px;
	border-bottom-left-radius: 3px;
	border-bottom-right-radius: 3px;
}
.translated-textarea{
	height:98%;
}
.translated-textarea textarea,p {
	height:100%;
}
.translated-textarea-tab{
	height:89%;
	padding-bottom: 0px !important;
}


================================================
FILE: styles/sprite-action-white.css
================================================
.icon-action-white {
	background-image: url('chrome-extension://__MSG_@@extension_id__/images/sprite-action-white.png');
}

.icon-action-white-ic_perm_identity_white_24dp {
	background-position: -5px -5px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_3d_rotation_white_24dp {
	background-position: -39px -5px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_account_balance_wallet_white_24dp {
	background-position: -5px -39px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_account_balance_white_24dp {
	background-position: -39px -39px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_account_box_white_24dp {
	background-position: -73px -5px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_account_child_white_24dp {
	background-position: -73px -39px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_account_circle_white_24dp {
	background-position: -5px -73px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_add_shopping_cart_white_24dp {
	background-position: -39px -73px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_alarm_add_white_24dp {
	background-position: -73px -73px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_alarm_off_white_24dp {
	background-position: -107px -5px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_alarm_on_white_24dp {
	background-position: -107px -39px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_alarm_white_24dp {
	background-position: -107px -73px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_android_white_24dp {
	background-position: -5px -107px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_announcement_white_24dp {
	background-position: -39px -107px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_aspect_ratio_white_24dp {
	background-position: -73px -107px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_assessment_white_24dp {
	background-position: -107px -107px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_assignment_ind_white_24dp {
	background-position: -141px -5px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_assignment_late_white_24dp {
	background-position: -141px -39px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_assignment_return_white_24dp {
	background-position: -141px -73px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_assignment_returned_white_24dp {
	background-position: -141px -107px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_assignment_turned_in_white_24dp {
	background-position: -5px -141px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_assignment_white_24dp {
	background-position: -39px -141px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_autorenew_white_24dp {
	background-position: -73px -141px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_backup_white_24dp {
	background-position: -107px -141px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_book_white_24dp {
	background-position: -141px -141px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_bookmark_outline_white_24dp {
	background-position: -175px -5px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_bookmark_white_24dp {
	background-position: -175px -39px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_bug_report_white_24dp {
	background-position: -175px -73px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_cached_white_24dp {
	background-position: -175px -107px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_check_circle_white_24dp {
	background-position: -175px -141px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_class_white_24dp {
	background-position: -5px -175px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_credit_card_white_24dp {
	background-position: -39px -175px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_dashboard_white_24dp {
	background-position: -73px -175px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_delete_white_24dp {
	background-position: -107px -175px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_description_white_24dp {
	background-position: -141px -175px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_dns_white_24dp {
	background-position: -175px -175px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_done_all_white_24dp {
	background-position: -209px -5px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_done_white_24dp {
	background-position: -209px -39px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_event_white_24dp {
	background-position: -209px -73px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_exit_to_app_white_24dp {
	background-position: -209px -107px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_explore_white_24dp {
	background-position: -209px -141px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_extension_white_24dp {
	background-position: -209px -175px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_face_white_24dp {
	background-position: -5px -209px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_favorite_outline_white_24dp {
	background-position: -39px -209px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_favorite_white_24dp {
	background-position: -73px -209px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_find_in_page_white_24dp {
	background-position: -107px -209px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_find_replace_white_24dp {
	background-position: -141px -209px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_flip_to_back_white_24dp {
	background-position: -175px -209px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_flip_to_front_white_24dp {
	background-position: -209px -209px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_get_app_white_24dp {
	background-position: -243px -5px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_grade_white_24dp {
	background-position: -243px -39px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_group_work_white_24dp {
	background-position: -243px -73px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_help_white_24dp {
	background-position: -243px -107px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_highlight_remove_white_24dp {
	background-position: -243px -141px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_history_white_24dp {
	background-position: -243px -175px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_home_white_24dp {
	background-position: -243px -209px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_https_white_24dp {
	background-position: -5px -243px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_info_outline_white_24dp {
	background-position: -39px -243px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_info_white_24dp {
	background-position: -73px -243px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_input_white_24dp {
	background-position: -107px -243px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_invert_colors_white_24dp {
	background-position: -141px -243px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_label_outline_white_24dp {
	background-position: -175px -243px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_label_white_24dp {
	background-position: -209px -243px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_language_white_24dp {
	background-position: -243px -243px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_launch_white_24dp {
	background-position: -277px -5px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_list_white_24dp {
	background-position: -277px -39px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_lock_open_white_24dp {
	background-position: -277px -73px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_lock_outline_white_24dp {
	background-position: -277px -107px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_lock_white_24dp {
	background-position: -277px -141px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_loyalty_white_24dp {
	background-position: -277px -175px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_markunread_mailbox_white_24dp {
	background-position: -277px -209px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_note_add_white_24dp {
	background-position: -277px -243px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_open_in_browser_white_24dp {
	background-position: -5px -277px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_open_in_new_white_24dp {
	background-position: -39px -277px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_open_with_white_24dp {
	background-position: -73px -277px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_pageview_white_24dp {
	background-position: -107px -277px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_payment_white_24dp {
	background-position: -141px -277px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_perm_camera_mic_white_24dp {
	background-position: -175px -277px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_perm_contact_cal_white_24dp {
	background-position: -209px -277px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_perm_data_setting_white_24dp {
	background-position: -243px -277px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_perm_device_info_white_24dp {
	background-position: -277px -277px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_accessibility_white_24dp {
	background-position: -311px -5px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_perm_media_white_24dp {
	background-position: -311px -39px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_perm_phone_msg_white_24dp {
	background-position: -311px -73px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_perm_scan_wifi_white_24dp {
	background-position: -311px -107px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_picture_in_picture_white_24dp {
	background-position: -311px -141px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_polymer_white_24dp {
	background-position: -311px -175px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_print_white_24dp {
	background-position: -311px -209px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_query_builder_white_24dp {
	background-position: -311px -243px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_question_answer_white_24dp {
	background-position: -311px -277px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_receipt_white_24dp {
	background-position: -5px -311px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_redeem_white_24dp {
	background-position: -39px -311px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_reorder_white_24dp {
	background-position: -73px -311px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_report_problem_white_24dp {
	background-position: -107px -311px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_restore_white_24dp {
	background-position: -141px -311px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_room_white_24dp {
	background-position: -175px -311px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_schedule_white_24dp {
	background-position: -209px -311px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_search_white_24dp {
	background-position: -243px -311px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_settings_applications_white_24dp {
	background-position: -277px -311px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_settings_backup_restore_white_24dp {
	background-position: -311px -311px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_settings_bluetooth_white_24dp {
	background-position: -345px -5px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_settings_cell_white_24dp {
	background-position: -345px -39px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_settings_display_white_24dp {
	background-position: -345px -73px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_settings_ethernet_white_24dp {
	background-position: -345px -107px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_settings_input_antenna_white_24dp {
	background-position: -345px -141px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_settings_input_component_white_24dp {
	background-position: -345px -175px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_settings_input_composite_white_24dp {
	background-position: -345px -209px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_settings_input_hdmi_white_24dp {
	background-position: -345px -243px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_settings_input_svideo_white_24dp {
	background-position: -345px -277px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_settings_overscan_white_24dp {
	background-position: -345px -311px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_settings_phone_white_24dp {
	background-position: -5px -345px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_settings_power_white_24dp {
	background-position: -39px -345px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_settings_remote_white_24dp {
	background-position: -73px -345px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_settings_voice_white_24dp {
	background-position: -107px -345px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_settings_white_24dp {
	background-position: -141px -345px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_shop_two_white_24dp {
	background-position: -175px -345px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_shop_white_24dp {
	background-position: -209px -345px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_shopping_basket_white_24dp {
	background-position: -243px -345px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_shopping_cart_white_24dp {
	background-position: -277px -345px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_speaker_notes_white_24dp {
	background-position: -311px -345px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_spellcheck_white_24dp {
	background-position: -345px -345px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_star_rate_white_24dp {
	background-position: -379px -5px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_stars_white_24dp {
	background-position: -379px -39px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_store_white_24dp {
	background-position: -379px -73px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_subject_white_24dp {
	background-position: -379px -107px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_supervisor_account_white_24dp {
	background-position: -379px -141px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_swap_horiz_white_24dp {
	background-position: -379px -175px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_swap_vert_circle_white_24dp {
	background-position: -379px -209px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_swap_vert_white_24dp {
	background-position: -379px -243px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_system_update_tv_white_24dp {
	background-position: -379px -277px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_tab_unselected_white_24dp {
	background-position: -379px -311px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_tab_white_24dp {
	background-position: -379px -345px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_theaters_white_24dp {
	background-position: -5px -379px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_thumb_down_white_24dp {
	background-position: -39px -379px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_thumb_up_white_24dp {
	background-position: -73px -379px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_thumbs_up_down_white_24dp {
	background-position: -107px -379px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_toc_white_24dp {
	background-position: -141px -379px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_today_white_24dp {
	background-position: -175px -379px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_track_changes_white_24dp {
	background-position: -209px -379px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_translate_white_24dp {
	background-position: -243px -379px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_trending_down_white_24dp {
	background-position: -277px -379px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_trending_neutral_white_24dp {
	background-position: -311px -379px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_trending_up_white_24dp {
	background-position: -345px -379px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_turned_in_not_white_24dp {
	background-position: -379px -379px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_turned_in_white_24dp {
	background-position: -413px -5px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_verified_user_white_24dp {
	background-position: -413px -39px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_view_agenda_white_24dp {
	background-position: -413px -73px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_view_array_white_24dp {
	background-position: -413px -107px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_view_carousel_white_24dp {
	background-position: -413px -141px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_view_column_white_24dp {
	background-position: -413px -175px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_view_day_white_24dp {
	background-position: -413px -209px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_view_headline_white_24dp {
	background-position: -413px -243px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_view_list_white_24dp {
	background-position: -413px -277px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_view_module_white_24dp {
	background-position: -413px -311px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_view_quilt_white_24dp {
	background-position: -413px -345px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_view_stream_white_24dp {
	background-position: -413px -379px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_view_week_white_24dp {
	background-position: -5px -413px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_visibility_off_white_24dp {
	background-position: -39px -413px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_visibility_white_24dp {
	background-position: -73px -413px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_wallet_giftcard_white_24dp {
	background-position: -107px -413px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_wallet_membership_white_24dp {
	background-position: -141px -413px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_wallet_travel_white_24dp {
	background-position: -175px -413px;
	width: 24px;
	height: 24px;
}

.icon-action-white-ic_work_white_24dp {
	background-position: -209px -413px;
	width: 24px;
	height: 24px;
}

