Speculos captures referenced from the pull request that gives the touch stack
one component for "choose among N". Screens only, kept out of the application
history.

## The comparison, at the repository root

`before-*` is the tree as it was: the menu, the BIP-39 phrase length and the
PIN length drawn as hand-built stacks of buttons, and the BIP-85 secret list
already drawn as the SDK's list. `after-*` is all four drawn as that list.

| screen | before | after |
|---|---|---|
| menu | `before-menu-<device>.png` | `after-menu-<device>.png` |
| BIP-39 phrase length | `before-phrase-length-<device>.png` | `after-phrase-length-<device>.png` |
| BIP-85 secret list | `before-which-secret-<device>.png` | `after-which-secret-<device>.png` |
| PIN length | `before-pin-length-<device>.png` | `after-pin-length-<device>.png` |

`<device>` is `stax`, `flex` or `apex_p`. The secret list is in the set
although it did not change: it is what the other three now look like.

## The four journeys, under `journeys/`

Every screen of each walk, in order, on the same three devices, on the tree as
it is now -- so that the four changed screens can be read in the company they
are actually seen in. Named `<journey>-<NN>-<screen>-<device>.png`, where NN is
the step's position in the walk.

| journey | steps | what it ends on |
|---|---|---|
| `check` | 5 | the verdict on a phrase typed in |
| `backup` | 11 | the review of the Shares, and its warning |
| `recover` | 4 | the ByteWords keyboard |
| `derive` | 11 | a derived PIN, and the home page it is closed to |

The Backup and Derive walks are the two that pass through more than one of the
changed screens.
