![bannerboxadventure](https://github.com/Taverna-Hub/BoxAdventure/assets/117609505/daad1520-c5a6-401f-8cba-63c8037826b7)

<p align="center"><b>BoxAdventure</b> é um runner game onde o jogador precisa desviar dos inimigos enquanto sacrifica "boxes" para ganhar pontos de vida.</p>

---

# ⚙️ Como rodar:

1º)  Clone o repositório
```
git clone https://github.com/Taverna-Hub/BoxAdventure.git
```

2º)  Entre na pasta do repositório
```
cd BoxAdventure/
```

3º)  Crie o arquivo que vai armazenar as pontuações
```
touch scores.txt
```

4º)  Compile os arquivos
```
gcc ./src/*.c -I./include -o BoxAdventure.out
```

5º)  Rode o executável do jogo
```
./BoxAdventure.out
```

<br>

# 🕹️ Como jogar:
O objetivo do jogador é chegar o mais longe que ele conseguir. Quanto mais pontos ele ganha, mais desafiador o jogo fica.

O jogo possui 3 "personagens" principais:
- O jogador: 🔵
- O inimigo: 🔥
- A box: ⬜

## Os Inimigos
A mecânica é simples, se o 🔵 encostar no 🔥 e o jogador estiver com apenas 1 <b>Ponto de Vida</b>, o jogo acaba.

Para evitar que isso aconteça, o jogador deve apertar <b>[ ESPAÇO ]</b> para pular por cima do inimigo. 

![2024-05-2321-22-42 remuxedonline-video-cutter com-ezgif com-video-to-gif-converter](https://github.com/Taverna-Hub/BoxAdventure/assets/117609505/c26381f7-a06c-47e2-b0d6-77794e2a6ff3)

A cada pulo bem sucedido, o jogador ganha +100 pontos.

## As Boxes e os Pontos de Vida
<p>A essência do jogo está no sistema de Boxes e Pontos de Vida.</p

<p>O jogador começa com apenas 1 vida.</p> <img src="https://github.com/Taverna-Hub/BoxAdventure/assets/117609505/46de09a2-e1fd-4885-9d61-c00aa12ac328"><br><br>

<p>Quando um jogador pula em uma box, ele rouba a sua energia vital, matando-a no processo e ganhando +200 pontos.</p>

![2024-05-2321-45-20 remuxedonline-video-cutter com1-ezgif com-video-to-gif-converter](https://github.com/Taverna-Hub/BoxAdventure/assets/117609505/1550f8ec-7b07-4fc0-ab21-7f8ff9091f62)

<p>A energia vital da box é convertida em 1 ou mais <b>Pedaços de Vida</b> para o jogador. Ao juntar 5 Pedaços, ele ganha 1 <b>Ponto de Vida</b>.</p>
<div>
   
   ![image](https://github.com/Taverna-Hub/BoxAdventure/assets/117609505/b8b1ebd6-8482-4168-bbb7-fb75a407ab23)
   ![image](https://github.com/Taverna-Hub/BoxAdventure/assets/117609505/9ab78505-7138-4312-bd5e-36bcb42ccc3e)
</div>

<p>Então, para o jogador conseguir sobreviver por mais tempo, ele precisa se tornar um assassino e matar cada vez mais boxes.</p>

<p>Mas lembre-se, matar é um pecado, e você pagará por isso...</p>

<br>

# 👥 Equipe:
<table align="center">
<tr align="center">
   <td align="center" style="word-wrap: break-word; width: 150.0; height: 150.0">
        <a href="https://github.com/gustavoyoq">
        <div  
          style="border: 3px solid #7f1d1d; border-radius: 50%; width: 115px; height: 115px; display: flex; align-items: center; justify-content: center;">
            <img src="https://github.com/Taverna-Hub/BoxAdventure/assets/117609505/7df3694d-4676-4980-b701-fecbe466454e" style="border-radius:50%;align-items:center;justify-content:center;overflow:hidden; width: 150px; " alt="Gustavo"/>
        </div>
            <br />
            <sub style="font-size:14px"><b>Gustavo</b></sub>
        </a>
    </td>
    <td align="center" style="word-wrap: break-word; width: 150.0; height: 150.0">
        <a href="https://github.com/Kaiquegb">
            <div style="border: 3px solid #7f1d1d; border-radius: 50%; width: 115px; height: 115px; display: flex; align-items: center; justify-content: center;">
              <img src="https://github.com/Taverna-Hub/BoxAdventure/assets/117609505/433db23f-2f92-4132-a25d-8c017384b49d" style="border-radius:50%;align-items:center;justify-content:center;overflow:hidden; width: 150px; " alt="Luan"/>
            </div>
            <br />
            <sub style="font-size:14px;"><b>Kaique</b></sub>
        </a>
    </td>
    <td align="center" style="word-wrap: break-word; width: 150.0; height: 150.0">
        <a href="https://github.com/paulorosadodev">
        <div  
          style="border: 3px solid #7f1d1d; border-radius: 50%; width: 115px; height: 115px; display: flex; align-items: center; justify-content: center;">
            <img src="https://github.com/Taverna-Hub/BoxAdventure/assets/117609505/f56051c7-fe59-43a8-8da8-57976ee5835e" style="border-radius:50%;align-items:center;justify-content:center;overflow:hidden; width: 150px; " alt="Paulo"/>
        </div>
            <br />
            <sub style="font-size:14px"><b>Paulo</b></sub>
        </a>
    </td>
</tr>
</table>



<br>
<br>
<p align="center"><img src="https://github.com/Taverna-Hub/ForgeSheets/assets/117609505/9287638f-8716-4e62-9989-a40882fbfed6" width="40px"></p>
<p align="center">Projeto para a disciplina de <strong>Programação Imperativa e Funcional</strong> na <strong>CESAR School‎‎</strong></p>
