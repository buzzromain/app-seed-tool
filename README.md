Speculos captures referenced from the pull request that gives the touch stack
one component for "choose among N". Screens only, kept out of the application
history.

`before-*` is the tree as it was: the menu, the BIP-39 phrase length and the
PIN length drawn as hand-built stacks of buttons, and the BIP-85 secret list
already drawn as the SDK's list. `after-*` is all four drawn as that list.

Four screens, three devices:

| screen | before | after |
|---|---|---|
| menu | `before-menu-<device>.png` | `after-menu-<device>.png` |
| BIP-39 phrase length | `before-phrase-length-<device>.png` | `after-phrase-length-<device>.png` |
| BIP-85 secret list | `before-which-secret-<device>.png` | `after-which-secret-<device>.png` |
| PIN length | `before-pin-length-<device>.png` | `after-pin-length-<device>.png` |

`<device>` is `stax`, `flex` or `apex_p`. The secret list is in the set
although it did not change: it is what the other three now look like.
