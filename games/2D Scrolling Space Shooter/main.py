import pygame
import os, sys
import time
import random

pygame.init()
pygame.font.init()

WIDTH, HEIGHT = 750, 750
screen = pygame.display.set_mode((WIDTH, HEIGHT))
pygame.display.set_caption("Space Shooter Tutorial")

# Load Font
font_path = os.path.join("assets", "fonts", "MuseoModerno-VariableWeight.ttf")

# Load images
enemy_path = os.path.join("assets", "PNG", "Enemies")
enemy_files = os.listdir(enemy_path)
# create a separate list for each color of enemy
ENEMIES_RED, ENEMIES_BLUE, ENEMIES_GREEN, ENEMIES_BLACK = ([], [], [], [])
for file_name in enemy_files:
    if "Black" in file_name:
        ENEMIES_BLACK.append(pygame.image.load(os.path.join(enemy_path, file_name)))
    elif "Blue" in file_name:
        ENEMIES_BLUE.append(pygame.image.load(os.path.join(enemy_path, file_name)))
    elif "Green" in file_name:
        ENEMIES_GREEN.append(pygame.image.load(os.path.join(enemy_path, file_name)))
    else:
        ENEMIES_RED.append(pygame.image.load(os.path.join(enemy_path, file_name)))

# Background
BG = pygame.image.load(os.path.join("assets", "Backgrounds", "darkPurple.png"))
BG = pygame.transform.scale(BG, (WIDTH, HEIGHT))

# top left
BG1_y = 0
BG1_x = 0
# bottom left
BG2_y = BG.get_height()
BG2_x = BG1_x
# top right
BG3_y = BG1_y
BG3_x = BG.get_width()
# bottom right
BG4_y = BG.get_height()
BG4_x = BG.get_width()

# Player sprite
PLAYER_SHIP = pygame.image.load(os.path.join("assets", "PNG", "playerShip3_red.png"))

# Lasers
RED_LASER = pygame.image.load(os.path.join("assets", "PNG", "Lasers", "laserRed02.png"))
GREEN_LASER = pygame.image.load(os.path.join("assets", "PNG", "Lasers", "laserGreen02.png"))
BLUE_LASER = pygame.image.load(os.path.join("assets", "PNG", "Lasers", "laserBlue02.png"))


class Laser:
    def __init__(self, x, y, img):
        self.x = x
        self.y = y
        self.img = img
        self.mask = pygame.mask.from_surface(self.img)

    def draw(self, window):
        window.blit(self.img, (self.x, self.y))

    def move(self, vel):
        self.y += vel

    def off_screen(self, height):
        return not (self.y <= height and self.y >= 0)

    def collision(self, obj):
        return collide(self, obj)


class Ship:
    COOLDOWN = 30

    def __init__(self, x, y, health=100):
        self.x = x
        self.y = y
        self.health = health
        self.ship_img = None
        self.laser_img = None
        self.lasers = []
        self.cool_down_counter = 0

    def draw(self, window):
        window.blit(self.ship_img, (self.x, self.y))
        for laser in self.lasers:
            laser.draw(window)

    def move_lasers(self, vel, obj):
        self.cooldown()
        for laser in self.lasers:
            laser.move(vel)
            if laser.off_screen(HEIGHT):
                self.lasers.remove(laser)
            elif laser.collision(obj):
                obj.health -= 10
                self.lasers.remove(laser)

    def cooldown(self):
        if self.cool_down_counter >= self.COOLDOWN:
            self.cool_down_counter = 0
        elif self.cool_down_counter > 0:
            self.cool_down_counter += 1

    def shoot(self):
        if self.cool_down_counter == 0:
            # create a new laser centered horizontally on the ship
            laser = Laser(self.x, self.y, self.laser_img)
            self.lasers.append(laser)
            self.cool_down_counter = 1

    def get_width(self):
        return self.ship_img.get_width()

    def get_height(self):
        return self.ship_img.get_height()


class Player(Ship):
    def __init__(self, x, y, health=100):
        super().__init__(x, y, health)
        self.ship_img = PLAYER_SHIP
        self.laser_img = RED_LASER
        self.mask = pygame.mask.from_surface(self.ship_img)
        self.max_health = health

    def move_lasers(self, vel, objs):
        self.cooldown()
        for laser in self.lasers:
            laser.move(vel)
            if laser.off_screen(HEIGHT):
                self.lasers.remove(laser)
            else:
                for obj in objs:
                    if laser.collision(obj):
                        objs.remove(obj)
                        if laser in self.lasers:
                            self.lasers.remove(laser)

    def draw(self, window):
        super().draw(window)
        self.healthbar(window)

    def healthbar(self, window):
        pygame.draw.rect(window, (255, 0, 0),
                         (self.x, self.y + self.ship_img.get_height() + 10, self.ship_img.get_width(), 10))
        pygame.draw.rect(window, (0, 255, 0), (
            self.x, self.y + self.ship_img.get_height() + 10,
            self.ship_img.get_width() * (self.health / self.max_health),
            10))


class Enemy(Ship):
    COLOR_MAP = {
        "red": (random.choice(ENEMIES_RED), RED_LASER),
        "green": (random.choice(ENEMIES_GREEN), GREEN_LASER),
        "blue": (random.choice(ENEMIES_BLUE), BLUE_LASER),
        "black": (random.choice(ENEMIES_BLACK), RED_LASER)
    }

    def __init__(self, x, y, color, health=100):
        super().__init__(x, y, health)
        self.ship_img, self.laser_img = self.COLOR_MAP[color]
        self.mask = pygame.mask.from_surface(self.ship_img)

    def move(self, vel):
        self.y += vel

    def shoot(self):
        if self.cool_down_counter == 0:
            laser = Laser(self.x - 20, self.y, self.laser_img)
            self.lasers.append(laser)
            self.cool_down_counter = 1


def collide(obj1, obj2):
    offset_x = obj2.x - obj1.x
    offset_y = obj2.y - obj1.y
    return obj1.mask.overlap(obj2.mask, (offset_x, offset_y)) != None


def run_game():
    global BG1_y, BG2_y, BG3_y, BG4_y
    global BG1_x, BG2_x, BG3_x, BG4_x
    run = True
    FPS = 60
    level = 0
    lives = 5

    # use pygame.font.get_fonts() to get a list of available fonts
    main_font = pygame.font.Font(font_path, 50)
    lost_font = pygame.font.SysFont(font_path, 60)

    enemies = []
    wave_length = 5
    enemy_vel = 1

    player_vel = 5
    laser_vel = 5

    player = Player(300, 630)

    clock = pygame.time.Clock()

    lost = False
    lost_count = 0

    '''
    draw a 2D infinitely scrolling background using 4 identical and seamless background tiles
    '''
    def draw_background():
        global BG1_y, BG2_y, BG3_y, BG4_y
        global BG1_x, BG2_x, BG3_x, BG4_x

        # Reaching the edge of our background tiles, so reset them
        if BG1_x > BG.get_width() or BG1_x < -(BG.get_width()):
            BG1_x = 0

        if BG1_y > BG.get_height() or BG1_y < -(BG.get_height()):
            BG1_y = 0

        # BG1 is moving off the screen to the left, so we need to cover our right half
        # this is when the player is moving right
        if BG1_x < 0:
            BG3_x = BG1_x + BG.get_width()
            BG4_x = BG1_x + BG.get_width()
        # BG1 is moving off to the right, so we need to cover our left
        # player moving left
        elif BG1_x > 0:
            BG3_x = BG1_x - BG.get_width()
            BG4_x = BG1_x - BG.get_width()
        # BG1 is moving off to the top, so we cover our bottom
        if BG1_y < 0:
            BG2_y = BG1_y + BG.get_height()
            BG4_y = BG1_y + BG.get_height()
        # BG1 is moving off to the bottom, so we cover our top
        elif BG1_y > 0:
            BG2_y = BG1_y - BG.get_height()
            BG4_y = BG1_y - BG.get_height()
        # BG2 and BG3 always share an x and y coordinate with BG1, respectively
        BG2_x = BG1_x
        BG3_y = BG1_y

        # draw scrolling backgrounds
        screen.blit(BG, (BG1_x, BG1_y))
        screen.blit(BG, (BG2_x, BG2_y))
        screen.blit(BG, (BG3_x, BG3_y))
        screen.blit(BG, (BG4_x, BG4_y))

    def draw_gui():
        # draw text

        lives_label = main_font.render(f"Lives: {lives}", 1, (255, 255, 255))
        level_label = main_font.render(f"Level: {level}", 1, (255, 255, 255))

        screen.blit(lives_label, (10, 10))
        screen.blit(level_label, (WIDTH - level_label.get_width() - 10, 10))

    def redraw_window():
        draw_background()
        draw_gui()

        for enemy in enemies:
            enemy.draw(screen)

        player.draw(screen)

        if lost:
            lost_label = lost_font.render("You Lost!!", 1, (255, 255, 255))
            screen.blit(lost_label, (WIDTH / 2 - lost_label.get_width() / 2, 350))

        # update all the drawings on screen
        pygame.display.update()
        # # # # # # # # # # # #

    while run:
        clock.tick(FPS)
        redraw_window()

        if lives <= 0 or player.health <= 0:
            lost = True
            lost_count += 1

        if lost:
            if lost_count > FPS * 3:
                run = False
            else:
                continue

        if len(enemies) == 0:
            level += 1
            wave_length += 5
            for i in range(wave_length):
                enemy = Enemy(random.randrange(50, WIDTH - 100), random.randrange(-1500, -100),
                              random.choice(["red", "blue", "green", "black"]))
                enemies.append(enemy)

        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                quit()

        # Player Controls / Key Press Handling ---------------------------------------------------------------------- #
        keys = pygame.key.get_pressed()
        if keys[pygame.K_a]:  # 'a' key pressed
            if player.x - player_vel > 0:
                # if player not on edge of screen, move player left
                player.x -= player_vel
            BG1_x += player_vel
            print("player coords: (", player.x, ", ", player.y, ")")
            print("BG1 coords: (", BG1_x, ", ", BG1_y, ")")
        if keys[pygame.K_d]:  # 'd' key pressed
            if player.x + player_vel + player.get_width() < WIDTH:
                # if player not on edge of screen, move player right
                player.x += player_vel
            BG1_x -= player_vel
            print("player coords: (", player.x, ", ", player.y, ")")
            print("BG1 coords: (", BG1_x, ", ", BG1_y, ")")
        if keys[pygame.K_w]:  # 'w' key pressed
            if player.y - player_vel > 0:
                # if player not on edge of screen, move player up
                player.y -= player_vel
            BG1_y += player_vel
            print("player coords: (", player.x, ", ", player.y, ")")
            print("BG1 coords: (", BG1_x, ", ", BG1_y, ")")
        if keys[pygame.K_s]:  # 's' key pressed
            if player.y + player_vel + player.get_height() + 15 < HEIGHT:
                # if player not on edge of screen, move player down
                player.y += player_vel
            BG1_y -= player_vel
            print("player coords: (", player.x, ", ", player.y, ")")
            print("BG1 coords: (", BG1_x, ", ", BG1_y, ")")
        if keys[pygame.K_SPACE]:
            player.shoot()

        # Enemy Rendering ----------------------------------------------------------------------------------- #
        for enemy in enemies[:]:
            enemy.move(enemy_vel)
            enemy.move_lasers(laser_vel, player)

            if random.randrange(0, 2 * 60) == 1:
                enemy.shoot()

            if collide(enemy, player):
                player.health -= 10
                enemies.remove(enemy)
            elif enemy.y + enemy.get_height() > HEIGHT:
                lives -= 1
                enemies.remove(enemy)

        player.move_lasers(-laser_vel, enemies)


def main_menu():
    global font_path
    title_font = pygame.font.Font(font_path, 40)
    run = True
    while run:
        screen.blit(BG, (0, 0))
        title_label = title_font.render("Press SPACE to begin", 1, (255, 255, 255))
        screen.blit(title_label, (WIDTH / 2 - title_label.get_width() / 2, 350))
        pygame.display.update()
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                run = False
            if event.type == pygame.MOUSEBUTTONDOWN:
                run_game()
            if event.type == pygame.KEYDOWN:
                if event.key == pygame.K_SPACE:
                    run_game()
    pygame.quit()


main_menu()
