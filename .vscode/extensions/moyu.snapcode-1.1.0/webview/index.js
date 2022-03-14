;(function() {
  const vscode = acquireVsCodeApi()

  vscode.postMessage({
    type: 'getAndUpdateCacheAndSettings'
  })

  const snippetNode = document.getElementById('snippet')
  const sizeNode = document.getElementById('size')
  const snippetContainerNode = document.getElementById('snippet-container')
  const obturateur = document.getElementById('save')

  snippetContainerNode.style.opacity = '1'
  const oldState = vscode.getState()
  if (oldState && oldState.innerHTML) {
    snippetNode.innerHTML = oldState.innerHTML
  }

  function getInitialHtml(fontFamily) {
    const cameraWithFlashEmoji = String.fromCodePoint(128248)
    const monoFontStack = `${fontFamily},SFMono-Regular,Consolas,DejaVu Sans Mono,Ubuntu Mono,Liberation Mono,Menlo,Courier,monospace`

    return `<meta charset="utf-8"><div style="color: #d8dee9; font-family: ${monoFontStack};font-weight: normal;font-size: 12px;line-height: 18px;white-space: pre;"><div><span style="color: #8fbcbb;">console</span><span style="color: #eceff4;">.</span><span style="color: #88c0d0;">log</span><span style="color: #d8dee9;">(</span><span style="color: #eceff4;">'</span><span style="color: #a3be8c;">0. Run command \`Snapcode ${cameraWithFlashEmoji}\`</span><span style="color: #eceff4;">'</span><span style="color: #d8dee9;">)</span></div><div><span style="color: #8fbcbb;">console</span><span style="color: #eceff4;">.</span><span style="color: #88c0d0;">log</span><span style="color: #d8dee9;">(</span><span style="color: #eceff4;">'</span><span style="color: #a3be8c;">1. Copy some code</span><span style="color: #eceff4;">'</span><span style="color: #d8dee9;">)</span></div><div><span style="color: #8fbcbb;">console</span><span style="color: #eceff4;">.</span><span style="color: #88c0d0;">log</span><span style="color: #d8dee9;">(</span><span style="color: #eceff4;">'</span><span style="color: #a3be8c;">2. Paste into Snapcode view</span><span style="color: #eceff4;">'</span><span style="color: #d8dee9;">)</span></div><div><span style="color: #8fbcbb;">console</span><span style="color: #eceff4;">.</span><span style="color: #88c0d0;">log</span><span style="color: #d8dee9;">(</span><span style="color: #eceff4;">'</span><span style="color: #a3be8c;">3. Click the button ${cameraWithFlashEmoji}</span><span style="color: #eceff4;">'</span><span style="color: #d8dee9;">)</span></div></div></div>`
  }

  function serializeBlob(blob, cb) {
    const fileReader = new FileReader()

    fileReader.onload = () => {
      const bytes = new Uint8Array(fileReader.result)
      cb(Array.from(bytes).join(','))
    }

    fileReader.readAsArrayBuffer(blob)
  }

  function getSnippetBgColor(html) {
    const match = html.match(/background-color: (#[a-fA-F0-9]+)/)
    return match ? match[1] : undefined
  }

  function updateEnvironment(snippetBgColor) {
    // update snippet bg color
    document.getElementById('snippet').style.backgroundColor = snippetBgColor
  }

  function getMinIndent(code) {
    const arr = code.split('\n')

    let minIndentCount = Number.MAX_VALUE
    for (let i = 0; i < arr.length; i++) {
      const wsCount = arr[i].search(/\S/)
      if (wsCount !== -1) {
        if (wsCount < minIndentCount) {
          minIndentCount = wsCount
        }
      }
    }

    return minIndentCount
  }

  function stripInitialIndent(html, indent) {
    const doc = new DOMParser().parseFromString(html, 'text/html')
    const initialSpans = doc.querySelectorAll('div > div span:first-child')
    for (let i = 0; i < initialSpans.length; i++) {
      initialSpans[i].textContent = initialSpans[i].textContent.slice(indent)
    }
    return doc.body.innerHTML
  }

  function updateSizeInfo() {
    const width = snippetContainerNode.offsetWidth
    const height = snippetContainerNode.offsetHeight
    sizeNode.textContent = width + 'x' + height
  }

  window.addEventListener('resize', updateSizeInfo);

  document.addEventListener('paste', e => {
    const innerHTML = e.clipboardData.getData('text/html')
    const code = e.clipboardData.getData('text/plain')
    const minIndent = getMinIndent(code)

    const snippetBgColor = getSnippetBgColor(innerHTML)
    if (snippetBgColor) {
      vscode.postMessage({
        type: 'updateBgColor',
        data: {
          bgColor: snippetBgColor
        }
      })
      updateEnvironment(snippetBgColor)
    }

    if (minIndent !== 0) {
      snippetNode.innerHTML = stripInitialIndent(innerHTML, minIndent)
    } else {
      snippetNode.innerHTML = innerHTML
    }

    updateSizeInfo()
    vscode.setState({ innerHTML })
  })

  obturateur.addEventListener('click', () => {
    shoot()
  })

  function shoot() {
    const width = snippetContainerNode.offsetWidth * 2
    const height = snippetContainerNode.offsetHeight * 2
    const config = {
      width,
      height,
      style: {
        transform: 'scale(2)',
        'transform-origin': 'top left',
        background: 'none'
      }
    }

    sizeNode.style.display = 'none'
    domtoimage.toBlob(snippetContainerNode, config).then(blob => {
      sizeNode.style.display = ''
      serializeBlob(blob, serializedBlob => {
        vscode.postMessage({
          type: 'shoot',
          data: {
            serializedBlob
          }
        })
      })
    })
  }

  let isInAnimation = false

  obturateur.addEventListener('mouseover', () => {
    if (!isInAnimation) {
      isInAnimation = true

      new Vivus(
        'save',
        {
          duration: 40,
          onReady: () => {
            obturateur.className = 'obturateur filling'
          }
        },
        () => {
          setTimeout(() => {
            isInAnimation = false
            obturateur.className = 'obturateur'
          }, 700)
        }
      )
    }
  })

  window.addEventListener('message', e => {
    if (e) {
      if (e.data.type === 'init') {
        const { fontFamily } = e.data

        const initialHtml = getInitialHtml(fontFamily)
        snippetNode.innerHTML = initialHtml
        vscode.setState({ innerHTML: initialHtml })

      } else if (e.data.type === 'update') {
        document.execCommand('paste')
      } else if (e.data.type === 'restore') {
        snippetNode.innerHTML = e.data.innerHTML
        updateEnvironment(e.data.bgColor)
      } else if (e.data.type === 'restoreBgColor') {
        updateEnvironment(e.data.bgColor)
      } else if (e.data.type === 'updateSettings') {
        if (e.data.ligature) {
          snippetNode.style.fontVariantLigatures = 'normal'
        } else {
          snippetNode.style.fontVariantLigatures = 'none'
        }
      }
    }
  })
})()

